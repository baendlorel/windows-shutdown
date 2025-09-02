#include "render.h"

#include "app-state.h"
#include "i18n.h"
#include "ui.h"

void DrawToMemoryDC(HDC hdcMem, int w, int h, BYTE alpha) {
    auto& appState = AppState::getInstance();
    auto& i18n = I18N::getInstance();
    Graphics graphics(hdcMem);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);
    // Draw semi-transparent white background
    SolidBrush bgBrush(Color(77, 255, 255, 255));
    graphics.FillRectangle(&bgBrush, 0, 0, w, h);

    // Draw countdown in center
    if (appState.isCountingDown) {
        std::wstring countdownText = std::to_wstring(appState.countdownSeconds);
        std::wstring actionText;

        if (appState.isSleepCountdown) {
            actionText = L"Sleeping in ";
        } else if (appState.isRestartCountdown) {
            actionText = L"Restarting in ";
        } else {
            actionText = L"Shutting down in ";
        }

        std::wstring fullText = actionText + countdownText + L" seconds...";

        // Large font for countdown
        FontFamily fontFamily(L"Arial");
        Gdiplus::Font font(&fontFamily, COUNT_DOWN_FONT_SIZE, FontStyleBold);

        // Get text bounds for vertical centering
        RectF layoutRect(0, 0, (REAL)w, (REAL)h);
        RectF boundingBox;
        StringFormat format;
        format.SetAlignment(StringAlignmentCenter);
        graphics.MeasureString(fullText.c_str(), -1, &font, layoutRect, &format, &boundingBox);
        REAL y = (h - boundingBox.Height) / 2;

        // Draw main text with beautiful rendering
        DrawBeautifulText(graphics, fullText.c_str(), font, (REAL)w, y, Color(255, 255, 255, 255),
                          Color(128, 0, 0, 0));

        // Draw cancel instruction with beautiful rendering
        Gdiplus::Font smallFont(&fontFamily, INSTRUCTION_FONT_SIZE, FontStyleBold);
        std::wstring cancelText = L"Click anywhere or press any key to cancel";
        RectF cancelBounds;
        graphics.MeasureString(cancelText.c_str(), -1, &smallFont, layoutRect, &format,
                               &cancelBounds);
        REAL cancelY = y + boundingBox.Height + 20;
        DrawBeautifulText(graphics, cancelText.c_str(), smallFont, (REAL)w, cancelY,
                          Color(255, 220, 220, 220), Color(100, 0, 0, 0));
    } else {
        // Draw image buttons (original logic)
        for (int i = 0; i < BUTTON_COUNT; ++i) {
            int x = appState.buttons[i].x - appState.buttons[i].r;
            int y = appState.buttons[i].y - appState.buttons[i].r;
            int size = appState.buttons[i].r * 2;
            graphics.DrawImage(appState.buttons[i].png, x, y, size, size);
            // If hovered, overlay a semi-transparent white
            if (i == appState.hoveredIndex) {
                SolidBrush highlightBrush(Color(28, 255, 255, 255));
                graphics.FillEllipse(&highlightBrush, x, y, size, size);
                                     y + BUTTON_SHADOW_WIDTH, size - BUTTON_SHADOW_WIDTH * 2,
        }
    }

    // Draw instruction text below buttons
    FontFamily fontFamily(L"Arial");
    Gdiplus::Font instructionFont(&fontFamily, INSTRUCTION_FONT_SIZE, FontStyleBold);
    std::wstring instructionText = L"Press any key or click background to exit";

    RectF layoutRect(0, 0, (REAL)w, (REAL)h);
    RectF textBounds;
    StringFormat format;
    format.SetAlignment(StringAlignmentCenter);
    graphics.MeasureString(instructionText.c_str(), -1, &instructionFont, layoutRect, &format,
                           &textBounds);

    // Below buttons with some margin
    REAL textY = (REAL)((h / 2) + BUTTON_RADIUS + BUTTON_MARGIN_BOTTOM);

    // Draw text with beautiful rendering
    DrawBeautifulText(graphics, instructionText.c_str(), instructionFont, (REAL)w, textY,
                      Color(255, 200, 200, 200), Color(80, 0, 0, 0));
}

void DrawShadow(Graphics& graphics, const wchar_t* text, const Gdiplus::Font& font, REAL width,
                REAL y, const Color& color) {
    StringFormat format;
    format.SetAlignment(StringAlignmentCenter);
    format.SetLineAlignment(StringAlignmentNear);
    SolidBrush brush(Color(0, 0, 0, 0));
    for (int radius = 4; radius >= 1; radius -= 1) {
        int alpha = 220 / (radius + 1);
        brush.SetColor(Color(alpha, color.GetR(), color.GetG(), color.GetB()));
        for (int i = 0; i < 8; i++) {
            float dx = SHADOW_OFFSET[i][0] * radius;
            float dy = SHADOW_OFFSET[i][1] * radius;
            RectF layoutRect(0 + dx, y + dy, width, 1000);
            graphics.DrawString(text, -1, &font, layoutRect, &format, &brush);
        }
    }
}

void DrawBeautifulText(Graphics& graphics, const wchar_t* text, const Gdiplus::Font& font,
                       REAL width, REAL y, const Color& textColor, const Color& shadowColor) {
    graphics.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.SetCompositingQuality(CompositingQualityHighQuality);

    DrawShadow(graphics, text, font, width, y, shadowColor);

    SolidBrush textBrush(textColor);
    StringFormat format;
    format.SetAlignment(StringAlignmentCenter);
    format.SetLineAlignment(StringAlignmentNear);
    RectF layoutRect(0, y, width, 1000);
    graphics.DrawString(text, -1, &font, layoutRect, &format, &textBrush);
}

struct WH {
    int w;
    int h;
};

WH GetWH(HWND hWnd) {
    RECT rc;
    GetClientRect(hWnd, &rc);
    int w = rc.right - rc.left;
    int h = rc.bottom - rc.top;
    CenterButtons(w, h);
    return {w, h};
}

// todo remove alpha param
void UpdateLayered(HWND hWnd) {
    static auto& appState = AppState::getInstance();
    static WH wh = GetWH(hWnd);
    HDC hdcScreen = GetDC(NULL);
    HDC hdcMem = CreateCompatibleDC(hdcScreen);
    BITMAPINFO bmi = {0};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = wh.w;
    bmi.bmiHeader.biHeight = -wh.h;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    void* pvBits = nullptr;
    HBITMAP hBitmap = CreateDIBSection(hdcScreen, &bmi, DIB_RGB_COLORS, &pvBits, NULL, 0);
    if (hBitmap == NULL) {
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);
        return;
    }

    HGDIOBJ oldBmp = SelectObject(hdcMem, hBitmap);
    DrawToMemoryDC(hdcMem, wh.w, wh.h, appState.g_alpha);
    POINT ptWin = {0, 0};
    SIZE sizeWin = {wh.w, wh.h};
    BLENDFUNCTION blend = {AC_SRC_OVER, 0, appState.g_alpha, AC_SRC_ALPHA};
    UpdateLayeredWindow(hWnd, hdcScreen, &ptWin, &sizeWin, hdcMem, &ptWin, 0, &blend, ULW_ALPHA);
    SelectObject(hdcMem, oldBmp);
    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
    ReleaseDC(NULL, hdcScreen);
}