#include "render.h"

#include "app-state.h"
#include "i18n.h"

void DrawToMemoryDC(HDC hdcMem, int w, int h) {
    static auto& appState = AppState::GetInstance();
    static auto& i18n = I18N::GetInstance();
    static Gdiplus::SolidBrush bgBrush(BACKGROUND_COLOR);
    Gdiplus::Graphics graphics(hdcMem);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.SetTextRenderingHint(TextRenderingHint::TextRenderingHintAntiAlias);
    // Draw semi-transparent background
    graphics.FillRectangle(&bgBrush, 0, 0, w, h);

    // Draw countdown in center
    if (appState.isCountingDown()) {
        std::wstring countdownText = std::to_wstring(appState.countdownSeconds);
        LPCWSTR fullText = i18n.Wait(appState.action, appState.countdownSeconds).c_str();

        // Large font for countdown
        Gdiplus::FontFamily fontFamily(i18n.FontFamilyName.c_str());
        Gdiplus::Font font(&fontFamily, COUNT_DOWN_FONT_SIZE, FontStyleRegular);

        // Get text bounds for vertical centering
        Gdiplus::REAL rw = static_cast<Gdiplus::REAL>(w);
        Gdiplus::REAL rh = static_cast<Gdiplus::REAL>(h);
        Gdiplus::RectF layoutRect(0, 0, rw, rh);
        Gdiplus::RectF boundingBox;
        Gdiplus::StringFormat format;
        format.SetAlignment(StringAlignmentCenter);
        graphics.MeasureString(fullText, -1, &font, layoutRect, &format, &boundingBox);
        Gdiplus::REAL y = (h - boundingBox.Height) / 2;

        // Draw main text with beautiful rendering
        DrawText(graphics, fullText, font, rw, y, TEXT_COLOR, TEXT_SHADOW_COLOR);

        // Draw cancel instruction with beautiful rendering
        Gdiplus::Font smallFont(&fontFamily, INSTRUCTION_FONT_SIZE, FontStyleRegular);
        LPCWSTR cancelText = i18n.PressAnyKeyToCancel.c_str();
        Gdiplus::RectF cancelBounds;
        graphics.MeasureString(cancelText, -1, &smallFont, layoutRect, &format, &cancelBounds);
        Gdiplus::REAL cancelY = y + boundingBox.Height + 20;
        DrawText(graphics, cancelText, smallFont, rw, cancelY, TEXT_COLOR, TEXT_SHADOW_COLOR);
    } else {
        // Draw image buttons (original logic)
        for (int i = 0; i < BUTTON_COUNT; ++i) {
            int x = appState.buttons[i].x - appState.buttons[i].r + BUTTON_MARGIN_LEFT;
            int y = appState.buttons[i].y - appState.buttons[i].r + BUTTON_MARGIN_TOP;
            int size = appState.buttons[i].r * 2;
            graphics.DrawImage(appState.buttons[i].png, x, y, size, size);
            // If hovered, overlay a semi-transparent white
            if (i == appState.hoveredIndex) {
                Gdiplus::SolidBrush highlightBrush(BUTTON_HIGHLIGHT_COLOR);
                graphics.FillEllipse(&highlightBrush, x + BUTTON_SHADOW_WIDTH,
                                     y + BUTTON_SHADOW_WIDTH, size - BUTTON_SHADOW_WIDTH * 2,
                                     size - BUTTON_SHADOW_WIDTH * 2);
            }
        }

        // Draw instruction text below buttons
        Gdiplus::FontFamily fontFamily(i18n.FontFamilyName.c_str());
        Gdiplus::Font font(&fontFamily, INSTRUCTION_FONT_SIZE, FontStyleRegular);
        std::wstring instructionText = i18n.PressAnyKeyToExit;

        Gdiplus::REAL rw = static_cast<REAL>(w);
        Gdiplus::REAL rh = static_cast<REAL>(h);
        Gdiplus::RectF layoutRect(0, 0, rw, rh);
        Gdiplus::RectF textBounds;
        Gdiplus::StringFormat format;
        format.SetAlignment(StringAlignmentCenter);
        graphics.MeasureString(instructionText.c_str(), -1, &font, layoutRect, &format,
                               &textBounds);

        // Below buttons with some margin
        Gdiplus::REAL textY = static_cast<Gdiplus::REAL>((h / 2) + BUTTON_RADIUS +
                                                         BUTTON_MARGIN_TOP + BUTTON_MARGIN_BOTTOM);

        // Draw text with beautiful rendering
        DrawText(graphics, instructionText.c_str(), font, rw, textY,
                 Gdiplus::Color(255, 200, 200, 200), Gdiplus::Color(80, 0, 0, 0));
    }
}

void DrawShadow(Graphics& graphics, LPCWSTR text, const Gdiplus::Font& font, Gdiplus::REAL width,
                Gdiplus::REAL y, const Gdiplus::Color& color) {
    StringFormat format;
    format.SetAlignment(StringAlignmentCenter);
    format.SetLineAlignment(StringAlignmentNear);
    SolidBrush brush(Color(0, 0, 0, 0));
    for (int radius = 4; radius >= 1; radius -= 1) {
        int alpha = 80 / (radius + 1);
        brush.SetColor(Color(alpha, color.GetR(), color.GetG(), color.GetB()));
        for (int i = 0; i < 8; i++) {
            float dx = SHADOW_OFFSET[i][0] * radius;
            float dy = SHADOW_OFFSET[i][1] * radius;
            RectF layoutRect(0 + dx, y + dy, width, 1000);
            graphics.DrawString(text, -1, &font, layoutRect, &format, &brush);
        }
    }
}

void DrawText(Graphics& graphics, LPCWSTR text, const Gdiplus::Font& font, Gdiplus::REAL width,
              Gdiplus::REAL y, const Gdiplus::Color& textColor, const Gdiplus::Color& shadowColor) {
    graphics.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.SetCompositingQuality(CompositingQualityHighQuality);

    DrawShadow(graphics, text, font, width, y, shadowColor);

    Gdiplus::SolidBrush textBrush(textColor);
    Gdiplus::StringFormat format;
    format.SetAlignment(StringAlignmentCenter);
    format.SetLineAlignment(StringAlignmentNear);
    Gdiplus::RectF layoutRect(0, y, width, 1000);
    graphics.DrawString(text, -1, &font, layoutRect, &format, &textBrush);
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