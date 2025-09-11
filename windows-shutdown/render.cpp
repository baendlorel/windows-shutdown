#include "render.h"
#include <format>

#include "components/warning.h"
#include "views/countdown.h"
#include "views/donate.h"
#include "views/home.h"

#include "consts/font-style.h"
#include "app-state.h"
#include "i18n.h"
#include "ui.h"

void __DrawDebug(Gdiplus::Graphics& graphics, int w, int h) {
    static Gdiplus::FontFamily fontFamily(I18N::GetInstance().FontFamilyName.c_str());
    static Gdiplus::SolidBrush brush(Gdiplus::Color(255, 166, 0));
    static Gdiplus::StringFormat format;
    static const Gdiplus::RectF rect(w - 900, 20, w, h);
    static const Gdiplus::Font font =
        Gdiplus::Font(&fontFamily, INSTRUCTION_FONT_SIZE, Gdiplus::FontStyleBold);
    static auto& page = AppState::GetInstance().page;
    auto pageName = [](Page p) -> const wchar_t* {
        switch (p) {
            case Page::None:
                return L"None";
            case Page::Home:
                return L"Home";
            case Page::Countdown:
                return L"Countdown";
            case Page::Donate:
                return L"Donate";
            default:
                return L"Unknown";
        }
    };

    auto str = std::format(L"Home:{}, Cnt:{}, None:{}\nCur:{}, next:{}, fading:{}",
                           page.GetPageAlpha(Page::Home), page.GetPageAlpha(Page::Countdown),
                           page.GetPageAlpha(Page::None), pageName(page.current),
                           pageName(page.next), (page.fading ? L"true" : L"false"));
    graphics.DrawString(str.c_str(), -1, &font, rect, &format, &brush);
}

void DrawToMemoryDC(HDC hdcMem, int w, int h) {
    static auto& appState = AppState::GetInstance();
    static auto warningWStr = I18N::GetInstance().GetConfigWarningText(appState.config.warnings);
    static Gdiplus::Color baseBgColor = AppState::GetInstance().config.backgroundColor;
    static HomeView homeView;
    static CountdownView countdownView;
    static DonateView donateView;

    // Create a background brush with appState.windowPage.alpha applied
    Gdiplus::SolidBrush bgBrush(ApplyAlpha(&baseBgColor, appState.page.GetBackgroundAlpha()));

    Gdiplus::Graphics graphics(hdcMem);
    graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
    graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
    graphics.FillRectangle(&bgBrush, 0, 0, w, h);

    // * These functions will decide internally whether to draw based on current state
    if (!warningWStr.empty()) {
        DrawWarning(graphics, MAX_ALPHA, w, h, warningWStr);
    }

    // ! __DrawDebug(graphics, w, h);

    homeView.Draw(graphics, w, h);
    countdownView.Draw(graphics, w, h);
    donateView.Draw(graphics, w, h);
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
        appState.buttons[i].Center(buttonCount, i, w, h);
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