#include "render.h"
#include <format>

#include "consts/font-style.h"
#include "app-state.h"
#include "i18n.h"
#include "ui.h"

std::wstring FormatTime(int seconds) {
    int h = seconds / 3600;
    int m = (seconds % 3600) / 60;
    int s = seconds % 60;
    return std::format(L"{:02}:{:02}:{:02}", h, m, s);
}

Gdiplus::Color* ApplyAlpha(Gdiplus::Color* color, BYTE alpha) {
    if (alpha == TARGET_ALPHA) {
        return color;
    }

    // overallAlpha in 0..255, srcA in 0..255
    int blended = (alpha * color->GetA()) / 255;
    if (blended < 0) {
        blended = 0;
    } else if (blended > 255) {
        blended = 255;
    }
    Gdiplus::Color result(blended, color->GetR(), color->GetG(), color->GetB());
    return &result;
};

Gdiplus::ImageAttributes* ImageAttrWithAlpha(Gdiplus::Image* image, BYTE alpha) {
    if (alpha == TARGET_ALPHA) {
        return nullptr;
    }

    Gdiplus::ColorMatrix colorMatrix = {
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,           1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, alpha / 255.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
    Gdiplus::ImageAttributes imgAttr;
    imgAttr.SetColorMatrix(&colorMatrix);
    return &imgAttr;
}

// & Only draw instruction needs internal state check
void DrawInstruction(Gdiplus::Graphics& graphics, Gdiplus::RectF* rect, const std::wstring& text) {
    static bool showInstruction = AppState::GetInstance().config.instruction == Instruction::Show;
    if (!showInstruction) {
        return;
    }
    static auto& colors = ColorSet::GetInstance();
    static Gdiplus::FontFamily fontFamily(I18N::GetInstance().FontFamilyName.c_str());
    static Gdiplus::Font font(&fontFamily, INSTRUCTION_FONT_SIZE, Gdiplus::FontStyleBold);
    DrawTextParams instrParams = {.text = text,
                                  .font = &font,
                                  .rect = rect,
                                  .horizontalAlign = Gdiplus::StringAlignmentCenter,
                                  .color = &colors.TextColor,
                                  .shadowColor = &colors.TextShadowColor};
    DrawCachedUIText(graphics, instrParams);
}

void DrawWarning(Gdiplus::Graphics& graphics, int w, int h) {
    static auto& warnings = AppState::GetInstance().config.warnings;
    static auto& i18n = I18N::GetInstance();
    static auto& colors = ColorSet::GetInstance();
    static Gdiplus::FontFamily fontFamily(i18n.FontFamilyName.c_str());

    Gdiplus::Font warnFont(&fontFamily, INSTRUCTION_FONT_SIZE, Gdiplus::FontStyleBold);
    Gdiplus::RectF warnRect(CFG_WARNING_X, CFG_WARNING_Y, w, h);
    DrawTextParams warnParams = {.text = i18n.GetConfigWarningText(warnings),
                                 .font = &warnFont,
                                 .rect = &warnRect,
                                 .horizontalAlign = Gdiplus::StringAlignmentNear,
                                 .color = &colors.TextWarnColor,
                                 //  .color = &colors.TextDangerColor,
                                 .shadowColor = &colors.TextShadowColor};
    DrawCachedUIText(graphics, warnParams);
}

void DrawCountdown(Gdiplus::Graphics& graphics, int alpha, int w, int h) {
    static auto& appState = AppState::GetInstance();
    static auto& i18n = I18N::GetInstance();
    static auto& colors = ColorSet::GetInstance();

    static Gdiplus::FontFamily fontFamily(i18n.FontFamilyName.c_str());
    static auto* mono = Gdiplus::FontFamily::GenericMonospace();

    // First line: original style (left/right language preserved by i18n.Wait)
    std::wstring firstLine = i18n.Wait(appState.action, appState.countdownSeconds);

    // Fonts
    Gdiplus::Font firstFont(&fontFamily, COUNT_DOWN_FONT_SIZE, Gdiplus::FontStyleBold);
    // Position first line a bit above center
    Gdiplus::REAL y = h * 0.36f;
    Gdiplus::RectF firstRect(0, y, w, h);
    DrawTextParams firstParams = {.text = firstLine,
                                  .font = &firstFont,
                                  .rect = &firstRect,
                                  .horizontalAlign = Gdiplus::StringAlignmentCenter,
                                  .color = &colors.TextColor,
                                  .shadowColor = &colors.TextShadowColor};
    DrawCachedUIText(graphics, firstParams);

    // Second line: large centered numeric seconds
    std::wstring secondLine = FormatTime(appState.countdownSeconds) + i18n.Waiting[2];
    // Position second (number) centered below the first line
    Gdiplus::Font secondFont(mono, COUNT_DOWN_NUMBER_FONT_SIZE, Gdiplus::FontStyleBold);
    Gdiplus::RectF secondRect(0, y + 140, w, h);
    DrawTextParams secondParams = {.text = secondLine,
                                   .font = &secondFont,
                                   .rect = &secondRect,
                                   .manualAlign = false,
                                   .horizontalAlign = Gdiplus::StringAlignmentCenter,
                                   .color = &colors.TextWarnColor,
                                   .shadowColor = &colors.TextShadowColor};
    // & seconds cannot use cache since it changes every second
    DrawUIText(graphics, secondParams);

    // Draw cancel instruction below the number
    Gdiplus::RectF smallRect(0, y + 300, w, h);
    DrawInstruction(graphics, &smallRect, i18n.PressAnyKeyToCancel);
}

// alpha: 0-255 overall opacity multiplier for all painted colors in this function
void DrawButtons(Gdiplus::Graphics& graphics, int alpha, int w, int h) {
    static auto& appState = AppState::GetInstance();
    static auto& i18n = I18N::GetInstance();
    static auto& colors = ColorSet::GetInstance();
    if (alpha == 0) {
        return;
    }

    // Draw image buttons (original logic)
    for (int i = 0; i < appState.buttons.size(); ++i) {
        int x = appState.buttons[i].x - BUTTON_RADIUS;
        int y = appState.buttons[i].y - BUTTON_RADIUS;

        auto* attr = ImageAttrWithAlpha(appState.buttons[i].png, alpha);
        Gdiplus::Rect rect(x, y, BUTTON_DIAMETER, BUTTON_DIAMETER);
        graphics.DrawImage(appState.buttons[i].png, rect, 0, 0, BUTTON_DIAMETER, BUTTON_DIAMETER,
                           Gdiplus::UnitPixel, attr);
        // If hovered, overlay a semi-transparent white, but scaled by overall alpha
        if (i == appState.hoveredIndex) {
            Gdiplus::Color blended(*ApplyAlpha(&colors.ButtonHighlightColor, alpha));
            Gdiplus::SolidBrush highlightBrush(blended);
            graphics.FillEllipse(&highlightBrush, x + BUTTON_SHADOW_WIDTH, y + BUTTON_SHADOW_WIDTH,
                                 BUTTON_DIAMETER - BUTTON_SHADOW_WIDTH * 2,
                                 BUTTON_DIAMETER - BUTTON_SHADOW_WIDTH * 2);
        }
    }

    // Draw exit instruction below buttons
    int instrY = (h / 2) + BUTTON_RADIUS + BUTTON_MARGIN_TOP + BUTTON_MARGIN_BOTTOM;
    Gdiplus::RectF instrRect(0, instrY, w, h);
    DrawInstruction(graphics, &instrRect, i18n.PressAnyKeyToExit);
}

void DrawDonate(Gdiplus::Graphics& graphics, int w, int h) {
}

void DrawToMemoryDC(HDC hdcMem, int w, int h) {
    static auto& appState = AppState::GetInstance();
    static auto& colors = ColorSet::GetInstance();
    static Gdiplus::Color baseBgColor = AppState::GetInstance().config.backgroundColor;

    // Create a background brush with appState.windowPage.alpha applied
    Gdiplus::SolidBrush bgBrush(*ApplyAlpha(&baseBgColor, appState.windowPage.alpha));

    Gdiplus::Graphics graphics(hdcMem);
    graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
    graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
    graphics.FillRectangle(&bgBrush, 0, 0, w, h);

    // * These functions will decide internally whether to draw based on current state
    if (!appState.config.warnings.empty()) {
        DrawWarning(graphics, w, h);
    }

    if (appState.isCountingDown()) {
        DrawCountdown(graphics, w, h);
    } else {
        DrawButtons(graphics, appState.windowPage.alpha, w, h);
    }
}

struct WH {
    int w;
    int h;
};

// & Here we do not use appState.screenW/H.
// Although they are equivalent, we still need to write program that has
// more compilcated logic. Consider future features like responsive layout.
WH GetWH(HWND hWnd, AppState& appState) {
    RECT rc;
    GetClientRect(hWnd, &rc);
    int w = rc.right - rc.left;
    int h = rc.bottom - rc.top;
    int buttonCount = static_cast<int>(appState.buttons.size());
    float centerIndex = (buttonCount - 1) * 0.5f;

    for (short i = 0; i < buttonCount; i++) {
        int delta = static_cast<int>(BUTTON_CENTER_DISTANCE * (i - centerIndex));
        appState.buttons[i].Center(BUTTON_MARGIN_LEFT, BUTTON_MARGIN_TOP, w, h);
    }
    return {w, h};
}

void UpdateLayered(HWND hWnd) {
    static auto& appState = AppState::GetInstance();
    static WH wh = GetWH(hWnd, appState);
    HDC hdcScreen = GetDC(NULL);
    HDC hdcMem = CreateCompatibleDC(hdcScreen);

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
    BLENDFUNCTION blend = {AC_SRC_OVER, 0, appState.windowPage.alpha, AC_SRC_ALPHA};
    UpdateLayeredWindow(hWnd, hdcScreen, &ptWin, &sizeWin, hdcMem, &ptWin, 0, &blend, ULW_ALPHA);
    SelectObject(hdcMem, oldBmp);
    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
    ReleaseDC(NULL, hdcScreen);
}