#include "render.h"
#include <unordered_map>

#include "app-state.h"
#include "i18n.h"

// fixme instruction跑到最顶上去了
// fixme 点击按钮后按钮还在
// fixme 文字缓存的bitmap缺了半行

void DrawToMemoryDC(HDC hdcMem, int w, int h) {
    static auto& appState = AppState::GetInstance();
    static auto& i18n = I18N::GetInstance();
    static auto& colors = ColorSet::GetInstance();

    static Gdi::FontFamily fontFamily(i18n.FontFamilyName.c_str());
    static Gdi::SolidBrush bgBrush(colors.BackgroundColor);

    Gdi::Graphics graphics(hdcMem);
    graphics.SetSmoothingMode(Gdi::SmoothingModeAntiAlias);
    graphics.SetTextRenderingHint(Gdi::TextRenderingHintAntiAlias);
    // Draw semi-transparent background
    graphics.FillRectangle(&bgBrush, 0, 0, w, h);

    // Draw countdown in center
    if (appState.isCountingDown()) {
        std::wstring countdownText = std::to_wstring(appState.countdownSeconds);
        std::wstring fullText = i18n.Wait(appState.action, appState.countdownSeconds);

        // Large font for countdown
        Gdi::Font font(&fontFamily, COUNT_DOWN_FONT_SIZE, Gdi::FontStyleRegular);

        // Get text bounds for vertical centering
        Gdi::REAL rw = static_cast<Gdi::REAL>(w);
        Gdi::REAL rh = static_cast<Gdi::REAL>(h);
        Gdi::RectF layoutRect(0, 0, rw, rh);
        Gdi::RectF boundingBox;
        Gdi::StringFormat format;
        format.SetAlignment(Gdi::StringAlignmentCenter);
        graphics.MeasureString(fullText.c_str(), -1, &font, layoutRect, &format, &boundingBox);
        Gdi::REAL y = (h - boundingBox.Height) / 2;

        // Draw main text with beautiful rendering
        Gdi::RectF mainTextRect(0, y, rw, boundingBox.Height);
        DrawTextParams params = {.text = fullText,
                                 .font = &font,
                                 .rect = &mainTextRect,
                                 .horizontalAlign = Gdi::StringAlignmentCenter,
                                 .color = &colors.TextColor,
                                 .shadowColor = &colors.TextShadowColor};

        DrawUIText(graphics, params);

        // Draw cancel instruction with beautiful rendering
        Gdi::Font smallFont(&fontFamily, INSTRUCTION_FONT_SIZE, Gdi::FontStyleRegular);
        std::wstring cancelText = i18n.PressAnyKeyToCancel;
        Gdi::RectF cancelBounds;
        graphics.MeasureString(cancelText.c_str(), -1, &smallFont, layoutRect, &format,
                               &cancelBounds);
        Gdi::REAL cancelY = y + boundingBox.Height + 20;
        Gdi::RectF cancelTextRect(0, cancelY, rw, cancelBounds.Height);

        DrawTextParams smallParams = {.text = cancelText,
                                      .font = &smallFont,
                                      .rect = &cancelTextRect,
                                      .horizontalAlign = Gdi::StringAlignmentCenter,
                                      .color = &colors.TextColor,
                                      .shadowColor = &colors.TextShadowColor};
        DrawCachedUIText(graphics, smallParams);
        return;
    }

    // Not counting down
    auto& warnings = appState.config.warnings;
    if (!warnings.empty()) {
        Gdi::Font warnFont(&fontFamily, INSTRUCTION_FONT_SIZE, Gdi::FontStyleRegular);
        Gdi::RectF warnRect(CFG_WARNING_X, CFG_WARNING_Y, w - 20, 1000);
        DrawTextParams warnParams = {.text = i18n.GetConfigWarnings(warnings),
                                     .font = &warnFont,
                                     .rect = &warnRect,
                                     .horizontalAlign = Gdi::StringAlignmentNear,
                                     .color = &colors.TextWarnColor,
                                     .shadowColor = &colors.TextShadowColor};
        DrawCachedUIText(graphics, warnParams);
    }

    // Draw image buttons (original logic)
    for (int i = 0; i < BUTTON_COUNT; ++i) {
        int x = appState.buttons[i].x - appState.buttons[i].r + BUTTON_MARGIN_LEFT;
        int y = appState.buttons[i].y - appState.buttons[i].r + BUTTON_MARGIN_TOP;
        int size = appState.buttons[i].r * 2;
        graphics.DrawImage(appState.buttons[i].png, x, y, size, size);
        // If hovered, overlay a semi-transparent white
        if (i == appState.hoveredIndex) {
            Gdi::SolidBrush highlightBrush(colors.ButtonHighlightColor);
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
                                  .color = &colors.TextColor,
                                  .shadowColor = &colors.TextShadowColor};
    DrawCachedUIText(graphics, instrParams);
}

void DrawUITextShadow(Gdi::Graphics& graphics, DrawTextParams& params) {
    Gdi::StringFormat format;
    format.SetAlignment(params.horizontalAlign);
    format.SetLineAlignment(Gdi::StringAlignmentNear);
    Gdi::SolidBrush brush(Gdi::Color(0, 0, 0, 0));
    for (int radius = 4; radius >= 1; radius -= 1) {
        int alpha = 80 / (radius + 1);
        brush.SetColor(Gdi::Color(alpha, params.shadowColor->GetR(), params.shadowColor->GetG(),
                                  params.shadowColor->GetB()));
        for (int i = 0; i < 8; i++) {
            float dx = SHADOW_OFFSET[i][0] * radius;
            float dy = SHADOW_OFFSET[i][1] * radius;
            Gdi::RectF rect(params.rect->X + dx, params.rect->Y + dy, params.rect->Width,
                            params.rect->Height);
            graphics.DrawString(params.text.c_str(), -1, params.font, rect, &format, &brush);
        }
    }
}

void DrawUIText(Gdi::Graphics& graphics, DrawTextParams& params) {
    graphics.SetTextRenderingHint(Gdi::TextRenderingHintAntiAliasGridFit);
    graphics.SetSmoothingMode(Gdi::SmoothingModeAntiAlias);
    graphics.SetCompositingQuality(Gdi::CompositingQualityHighQuality);

    DrawUITextShadow(graphics, params);

    Gdi::SolidBrush brush(*params.color);
    Gdi::StringFormat format;
    format.SetAlignment(params.horizontalAlign);
    format.SetLineAlignment(Gdi::StringAlignmentNear);
    graphics.DrawString(params.text.c_str(), -1, params.font, *params.rect, &format, &brush);
}

Gdi::Bitmap* UITextToBitmap(Gdi::Graphics& graphics, DrawTextParams& params) {
    static std::unordered_map<std::wstring, Gdi::Bitmap*> cache;
    auto it = cache.find(params.text);
    if (it != cache.end()) {
        return it->second;
    }

    // Measure text size to determine bitmap dimensions
    Gdi::StringFormat format;
    format.SetAlignment(params.horizontalAlign);
    format.SetLineAlignment(Gdi::StringAlignmentNear);

    Gdi::RectF measureRect(0, 0, params.rect->Width, params.rect->Height);
    Gdi::RectF boundingBox;
    graphics.MeasureString(params.text.c_str(), -1, params.font, measureRect, &format,
                           &boundingBox);

    // Add extra margin for shadow (max radius is 4, need extra space in each direction)
    const int shadowMargin = 8;
    int bitmapWidth = static_cast<int>(boundingBox.Width) + shadowMargin * 2;
    int bitmapHeight = static_cast<int>(boundingBox.Height) + shadowMargin * 2;

    // Create new bitmap
    Gdi::Bitmap* bitmap = new Gdi::Bitmap(bitmapWidth, bitmapHeight, PixelFormat32bppARGB);
    Gdi::Graphics bitmapGraphics(bitmap);

    // Set high quality rendering
    bitmapGraphics.SetTextRenderingHint(Gdi::TextRenderingHintAntiAliasGridFit);
    bitmapGraphics.SetSmoothingMode(Gdi::SmoothingModeAntiAlias);
    bitmapGraphics.SetCompositingQuality(Gdi::CompositingQualityHighQuality);

    // Clear bitmap background to transparent
    bitmapGraphics.Clear(Gdi::Color(0, 0, 0, 0));

    // // Create draw params, adjust rect position to bitmap coordinates
    // DrawTextParams bitmapParams = params;
    // Gdi::RectF bitmapRect(shadowMargin, shadowMargin, boundingBox.Width, boundingBox.Height);
    // bitmapParams.rect = &bitmapRect;

    // // Draw shadow
    // DrawTextParams shadowParams = bitmapParams.GetShadowVersion();
    // DrawUITextShadow(bitmapGraphics, shadowParams);

    // // Draw main text
    // Gdi::SolidBrush brush(*bitmapParams.color);
    // bitmapGraphics.DrawString(bitmapParams.text, -1, bitmapParams.font, bitmapRect, &format,
    // &brush);
    DrawUIText(bitmapGraphics, params);

    // Store in cache
    cache[params.text] = bitmap;

    return bitmap;
}

void DrawCachedUIText(Gdi::Graphics& graphics, DrawTextParams& params) {
    Gdi::Bitmap* cachedBitmap = UITextToBitmap(graphics, params);
    if (!cachedBitmap) {
        return;
    }
    // Calculate draw position (consider shadow margin)
    const int shadowMargin = 8;
    Gdi::REAL drawX = params.rect->X - shadowMargin;
    Gdi::REAL drawY = params.rect->Y - shadowMargin;

    // Adjust X position according to alignment
    if (params.horizontalAlign == Gdi::StringAlignmentCenter) {
        drawX = params.rect->X + (params.rect->Width - cachedBitmap->GetWidth()) / 2;
    } else if (params.horizontalAlign == Gdi::StringAlignmentFar) {
        drawX = params.rect->X + params.rect->Width - cachedBitmap->GetWidth() + shadowMargin;
    }

    graphics.DrawImage(cachedBitmap, drawX, drawY);
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