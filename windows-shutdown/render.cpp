#include "render.h"
#include <format>

#include "consts/button-style.h"
#include "consts/effects.h"
#include "consts/font-style.h"
#include "consts/color-set.h"

#include "app-state.h"
#include "i18n.h"
#include "ui.h"

std::wstring FormatTime(int seconds) {
    static auto pad = [](int num) {
        return (num > 9) ? std::format(L"{}", num) : std::format(L"0{}", num);
    };

    int h = seconds / 3600;
    int m = (seconds % 3600) / 60;
    int s = seconds % 60;
    return std::format(L"{}:{}:{}", pad(h), pad(m), pad(s));
}

// & Only draw instruction needs internal state check
void DrawInstruction(Gdiplus::Graphics& graphics, BYTE alpha, Gdiplus::RectF* rect,
                     const std::wstring& text) {
    if (alpha == 0) {
        return;
    }

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
                                  .alpha = alpha,
                                  .color = &colors.TextColor,
                                  .shadowColor = &colors.TextShadowColor};
    DrawCachedUIText(graphics, instrParams);
}

void DrawWarning(Gdiplus::Graphics& graphics, BYTE alpha, int w, int h) {
    if (alpha == 0) {
        return;
    }

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
                                 .alpha = alpha,
                                 .color = &colors.TextWarnColor,
                                 //  .color = &colors.TextDangerColor,
                                 .shadowColor = &colors.TextShadowColor};
    DrawCachedUIText(graphics, warnParams);
}

void DrawCountdown(Gdiplus::Graphics& graphics, BYTE alpha, int w, int h) {
    if (alpha == 0) {
        return;
    }

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
                                  .alpha = alpha,
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
                                   .alpha = alpha,
                                   .color = &colors.TextWarnColor,
                                   .shadowColor = &colors.TextShadowColor};
    // & seconds cannot use cache since it changes every second
    DrawUIText(graphics, secondParams);

    // Draw cancel instruction below the number
    Gdiplus::RectF smallRect(0, y + 300, w, h);
    DrawInstruction(graphics, alpha, &smallRect, i18n.PressAnyKeyToCancel);
}

// alpha: 0-255 overall opacity multiplier for all painted colors in this function
void DrawButtons(Gdiplus::Graphics& graphics, BYTE alpha, int w, int h) {
    if (alpha == 0) {
        return;
    }

    static auto& appState = AppState::GetInstance();
    static auto& i18n = I18N::GetInstance();
    static auto& colors = ColorSet::GetInstance();

    // Draw image buttons (original logic)
    for (int i = 0; i < appState.buttons.size(); ++i) {
        auto& b = appState.buttons[i];
        int x = b.x - BUTTON_RADIUS;
        int y = b.y - BUTTON_RADIUS;

        auto imgAttr = ImageAttrWithAlpha(b.png, alpha);

        // where and what size to draw
        Gdiplus::Rect rect(x, y, BUTTON_DIAMETER, BUTTON_DIAMETER);

        // x, y, w, h cut from the source image
        // Since button images are 512x512, appState.buttons[i].png->GetWidth() is acutally 512
        graphics.DrawImage(b.png, rect, 0, 0, b.png->GetWidth(), b.png->GetHeight(),
                           Gdiplus::UnitPixel, imgAttr.get());

        // If hovered, overlay a semi-transparent white, but scaled by overall alpha
        if (i == appState.hoveredIndex) {
            Gdiplus::Color blended(ApplyAlpha(&colors.ButtonHighlightColor, alpha));
            Gdiplus::SolidBrush highlightBrush(blended);
            graphics.FillEllipse(&highlightBrush, x + BUTTON_SHADOW_WIDTH, y + BUTTON_SHADOW_WIDTH,
                                 BUTTON_DIAMETER - BUTTON_SHADOW_WIDTH * 2,
                                 BUTTON_DIAMETER - BUTTON_SHADOW_WIDTH * 2);
        }
    }

    // Draw exit instruction below buttons
    int instrY = (h / 2) + BUTTON_RADIUS + BUTTON_MARGIN_TOP + BUTTON_MARGIN_BOTTOM;
    Gdiplus::RectF instrRect(0, instrY, w, h);
    DrawInstruction(graphics, alpha, &instrRect, i18n.PressAnyKeyToExit);
}

void DrawDonate(Gdiplus::Graphics& graphics, int w, int h) {
}

void __DrawDebug(Gdiplus::Graphics& graphics, int w, int h) {
    static Gdiplus::FontFamily fontFamily(I18N::GetInstance().FontFamilyName.c_str());
    static Gdiplus::SolidBrush brush(Gdiplus::Color(255, 166, 0));
    static Gdiplus::StringFormat format;
    static const Gdiplus::RectF rect(w - 900, 20, w, h);
    static const Gdiplus::Font font =
        Gdiplus::Font(&fontFamily, INSTRUCTION_FONT_SIZE, Gdiplus::FontStyleBold);
    static auto& page = AppState::GetInstance().page;
    auto alphaStr = std::format(L"Home:{}, Cnt:{}, None:{}", page.GetPageAlpha(Page::Home),
                                page.GetPageAlpha(Page::Countdown), page.GetPageAlpha(Page::None));
    graphics.DrawString(alphaStr.c_str(), -1, &font, rect, &format, &brush);
}

void DrawToMemoryDC(HDC hdcMem, int w, int h) {
    static auto& appState = AppState::GetInstance();
    static auto& colors = ColorSet::GetInstance();
    static Gdiplus::Color baseBgColor = AppState::GetInstance().config.backgroundColor;

    // Create a background brush with appState.windowPage.alpha applied
    Gdiplus::SolidBrush bgBrush(ApplyAlpha(&baseBgColor, appState.page.GetBackgroundAlpha()));

    Gdiplus::Graphics graphics(hdcMem);
    graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
    graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
    graphics.FillRectangle(&bgBrush, 0, 0, w, h);

    // * These functions will decide internally whether to draw based on current state
    if (!appState.config.warnings.empty()) {
        DrawWarning(graphics, MAX_ALPHA, w, h);
    }

    __DrawDebug(graphics, w, h);

    DrawCountdown(graphics, appState.page.GetPageAlpha(Page::Countdown), w, h);
    DrawButtons(graphics, appState.page.GetPageAlpha(Page::Home), w, h);
}

/**
 * @brief  sadf
 */
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
        appState.buttons[i].Center(BUTTON_MARGIN_LEFT + delta, BUTTON_MARGIN_TOP, w, h);
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
    // appState.page.alpha
    BLENDFUNCTION blend = {AC_SRC_OVER, 0, MAX_ALPHA, AC_SRC_ALPHA};
    UpdateLayeredWindow(hWnd, hdcScreen, &ptWin, &sizeWin, hdcMem, &ptWin, 0, &blend, ULW_ALPHA);
    SelectObject(hdcMem, oldBmp);
    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
    ReleaseDC(NULL, hdcScreen);
}