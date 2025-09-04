#include "render.h"

#include "app-state.h"
#include "i18n.h"

DrawTextParams DrawTextParams::GetShadowVersion() {
    return {.text = this->text,
            .font = this->font,
            .rect = this->rect,
            .horizontalAlign = this->horizontalAlign,
            .color = this->shadowColor,
            .shadowColor = this->shadowColor};
}

void DrawToMemoryDC(HDC hdcMem, int w, int h) {
    static auto& appState = AppState::GetInstance();
    static auto& i18n = I18N::GetInstance();
    static Gdi::FontFamily fontFamily(i18n.FontFamilyName.c_str());
    static Gdi::SolidBrush bgBrush(BACKGROUND_COLOR);

    Gdi::Graphics graphics(hdcMem);
    graphics.SetSmoothingMode(Gdi::SmoothingModeAntiAlias);
    graphics.SetTextRenderingHint(Gdi::TextRenderingHintAntiAlias);
    // Draw semi-transparent background
    graphics.FillRectangle(&bgBrush, 0, 0, w, h);

    // Draw countdown in center
    if (appState.isCountingDown()) {
        std::wstring countdownText = std::to_wstring(appState.countdownSeconds);
        LPCWSTR fullText = i18n.Wait(appState.action, appState.countdownSeconds).c_str();

        // Large font for countdown
        Gdi::Font font(&fontFamily, COUNT_DOWN_FONT_SIZE, Gdi::FontStyleRegular);

        // Get text bounds for vertical centering
        Gdi::REAL rw = static_cast<Gdi::REAL>(w);
        Gdi::REAL rh = static_cast<Gdi::REAL>(h);
        Gdi::RectF layoutRect(0, 0, rw, rh);
        Gdi::RectF boundingBox;
        Gdi::StringFormat format;
        format.SetAlignment(Gdi::StringAlignmentCenter);
        graphics.MeasureString(fullText, -1, &font, layoutRect, &format, &boundingBox);
        Gdi::REAL y = (h - boundingBox.Height) / 2;

        // Draw main text with beautiful rendering
        Gdi::RectF mainTextRect(0, y, rw, boundingBox.Height);
        DrawTextParams params = {.text = fullText,
                                 .font = &font,
                                 .rect = &mainTextRect,
                                 .horizontalAlign = Gdi::StringAlignmentCenter,
                                 .color = &TEXT_COLOR,
                                 .shadowColor = &TEXT_SHADOW_COLOR};

        DrawUIText(graphics, params);

        // Draw cancel instruction with beautiful rendering
        Gdi::Font smallFont(&fontFamily, INSTRUCTION_FONT_SIZE, Gdi::FontStyleRegular);
        LPCWSTR cancelText = i18n.PressAnyKeyToCancel.c_str();
        Gdi::RectF cancelBounds;
        graphics.MeasureString(cancelText, -1, &smallFont, layoutRect, &format, &cancelBounds);
        Gdi::REAL cancelY = y + boundingBox.Height + 20;
        Gdi::RectF cancelTextRect(0, cancelY, rw, cancelBounds.Height);

        DrawTextParams smallParams = {.text = cancelText,
                                      .font = &smallFont,
                                      .rect = &cancelTextRect,
                                      .horizontalAlign = Gdi::StringAlignmentCenter,
                                      .color = &TEXT_COLOR,
                                      .shadowColor = &TEXT_SHADOW_COLOR};
        DrawUIText(graphics, smallParams);
    }

    auto& warnings = appState.config.warnings;
    if (!warnings.empty()) {
        LPCWSTR warnText = i18n.GetConfigWarnings(warnings).c_str();
        Gdi::Font warnFont(&fontFamily, INSTRUCTION_FONT_SIZE, Gdi::FontStyleRegular);
        Gdi::RectF warnRect(CFG_WARNING_X, CFG_WARNING_Y, w - 20, 1000);
        DrawTextParams warnParams = {.text = i18n.GetConfigWarnings(warnings).c_str(),
                                     .font = &warnFont,
                                     .rect = &warnRect,
                                     .horizontalAlign = Gdi::StringAlignmentCenter,
                                     .color = &TEXT_COLOR,
                                     .shadowColor = &TEXT_SHADOW_COLOR};
        DrawUIText(graphics, warnParams);
    }

    // Draw image buttons (original logic)
    for (int i = 0; i < BUTTON_COUNT; ++i) {
        int x = appState.buttons[i].x - appState.buttons[i].r + BUTTON_MARGIN_LEFT;
        int y = appState.buttons[i].y - appState.buttons[i].r + BUTTON_MARGIN_TOP;
        int size = appState.buttons[i].r * 2;
        graphics.DrawImage(appState.buttons[i].png, x, y, size, size);
        // If hovered, overlay a semi-transparent white
        if (i == appState.hoveredIndex) {
            Gdi::SolidBrush highlightBrush(BUTTON_HIGHLIGHT_COLOR);
            graphics.FillEllipse(&highlightBrush, x + BUTTON_SHADOW_WIDTH, y + BUTTON_SHADOW_WIDTH,
                                 size - BUTTON_SHADOW_WIDTH * 2, size - BUTTON_SHADOW_WIDTH * 2);
        }
    }

    // Draw instruction text below buttons
    Gdi::Font font(&fontFamily, INSTRUCTION_FONT_SIZE, Gdi::FontStyleRegular);
    LPCWSTR instr = i18n.PressAnyKeyToExit.c_str();

    Gdi::REAL rw = static_cast<Gdi::REAL>(w);
    Gdi::REAL rh = static_cast<Gdi::REAL>(h);
    Gdi::RectF layoutRect(0, 0, rw, rh);
    Gdi::RectF textBounds;
    Gdi::StringFormat format;
    format.SetAlignment(Gdi::StringAlignmentCenter);
    graphics.MeasureString(instr, -1, &font, layoutRect, &format, &textBounds);

    // Below buttons with some margin
    Gdi::REAL textY =
        static_cast<Gdi::REAL>((h / 2) + BUTTON_RADIUS + BUTTON_MARGIN_TOP + BUTTON_MARGIN_BOTTOM);

    // Draw text with beautiful rendering
    Gdi::RectF instrRect(0, textY, rw, textBounds.Height);

    DrawTextParams instrParams = {.text = instr,
                                  .font = &font,
                                  .rect = &layoutRect,
                                  .horizontalAlign = Gdi::StringAlignmentCenter,
                                  .color = &TEXT_COLOR,
                                  .shadowColor = &TEXT_SHADOW_COLOR};
    DrawUIText(graphics, instrParams);
}

void DrawUITextShadow(Gdi::Graphics& graphics, DrawTextParams& params) {
    Gdi::StringFormat format;
    format.SetAlignment(params.horizontalAlign);
    format.SetLineAlignment(Gdi::StringAlignmentNear);
    Gdi::SolidBrush brush(Gdi::Color(0, 0, 0, 0));
    for (int radius = 4; radius >= 1; radius -= 1) {
        int alpha = 80 / (radius + 1);
        brush.SetColor(
            Gdi::Color(alpha, params.color->GetR(), params.color->GetG(), params.color->GetB()));
        for (int i = 0; i < 8; i++) {
            float dx = SHADOW_OFFSET[i][0] * radius;
            float dy = SHADOW_OFFSET[i][1] * radius;
            Gdi::RectF rect(params.rect->X + dx, params.rect->Y + dy, params.rect->Width,
                            params.rect->Height);
            graphics.DrawString(params.text, -1, params.font, rect, &format, &brush);
        }
    }
}

void DrawUIText(Gdi::Graphics& graphics, DrawTextParams& params) {
    graphics.SetTextRenderingHint(Gdi::TextRenderingHintAntiAliasGridFit);
    graphics.SetSmoothingMode(Gdi::SmoothingModeAntiAlias);
    graphics.SetCompositingQuality(Gdi::CompositingQualityHighQuality);

    DrawTextParams shadowParams = params.GetShadowVersion();
    DrawUITextShadow(graphics, shadowParams);

    Gdi::SolidBrush brush(*params.color);
    Gdi::StringFormat format;
    format.SetAlignment(params.horizontalAlign);
    format.SetLineAlignment(Gdi::StringAlignmentNear);
    graphics.DrawString(params.text, -1, params.font, *params.rect, &format, &brush);
}

Gdi::Bitmap CacheUIText() {
}

struct WH {
    int w;
    int h;
};

WH GetWH(HWND hWnd, AppState& appState) {
    RECT rc;
    GetClientRect(hWnd, &rc);
    int w = rc.right - rc.left;
    int h = rc.bottom - rc.top;
    for (short i = 0; i < 5; i++) {
        appState.buttons[i].Center(w, h, i);
    }
    return {w, h};
}

void UpdateLayered(HWND hWnd) {
    static auto& appState = AppState::GetInstance();
    static WH wh = GetWH(hWnd, appState);
    HDC hdcScreen = GetDC(NULL);
    HDC hdcMem = CreateCompatibleDC(hdcScreen);
    // BITMAPINFO bmi = {0};
    BITMAPINFO bmi;
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = wh.w;
    bmi.bmiHeader.biHeight = -wh.h;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    void* pvBits = nullptr;
    HBITMAP hBitmap = CreateDIBSection(hdcScreen, &bmi, DIB_RGB_COLORS, &pvBits, NULL, 0);
    if (hBitmap == NULL) {
        static auto& i18n = I18N::GetInstance();
        MessageBoxW(nullptr, i18n.ErrCreateBitmap.c_str(), i18n.ErrTitle.c_str(), MB_ICONERROR);
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);
        return;
    }

    HGDIOBJ oldBmp = SelectObject(hdcMem, hBitmap);
    DrawToMemoryDC(hdcMem, wh.w, wh.h);
    POINT ptWin = {0, 0};
    SIZE sizeWin = {wh.w, wh.h};
    BLENDFUNCTION blend = {AC_SRC_OVER, 0, appState.g_alpha, AC_SRC_ALPHA};
    UpdateLayeredWindow(hWnd, hdcScreen, &ptWin, &sizeWin, hdcMem, &ptWin, 0, &blend, ULW_ALPHA);
    SelectObject(hdcMem, oldBmp);
    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
    ReleaseDC(NULL, hdcScreen);
}