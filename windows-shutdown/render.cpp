#include "render.h"
#include <format>
#include "style.font.h"

#include "mini-ui.h"
#include "components.warning.h"

void Renderer::__DrawDebug(Gdiplus::Graphics& graphics, int w, int h) {
    static auto& app = App::GetInstance();
    static auto& index = Index::GetInstance();
    static Gdiplus::FontFamily fontFamily(app.i18n.FontFamilyName.c_str());
    static Gdiplus::SolidBrush brush(Gdiplus::Color(255, 166, 0));
    static Gdiplus::StringFormat format;
    static const Gdiplus::RectF rect(w - 900, 20, w, h);
    static const Gdiplus::Font font =
        Gdiplus::Font(&fontFamily, INSTRUCTION_FONT_SIZE, Gdiplus::FontStyleBold);
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

    auto menuActive = -1;
    auto __drawingalpha = -1;

    if (index.home.menu.size() > 0) {
        menuActive = index.home.menu[0].IsActive() ? 1 : 0;
        __drawingalpha = index.home.menu[0].__drawingalpha;
    }

    static int count = 0;
    count++;
    auto str = std::format(
        L"{}, Home:{} (menu active: {}, {}), Cnt:{}, None:{}\nCur:{}, next:{}, fading:{}", count,
        app.page.GetPageAlpha(Page::Home), menuActive, __drawingalpha,
        app.page.GetPageAlpha(Page::Countdown), app.page.GetPageAlpha(Page::None),
        pageName(app.page.current), pageName(app.page.next),
        (app.page.fading ? L"true" : L"false"));

    graphics.DrawString(str.c_str(), -1, &font, rect, &format, &brush);
}

void Renderer::DrawToMemoryDC(HDC hdcMem, int w, int h) {
    static auto& app = App::GetInstance();
    static auto& index = Index::GetInstance();
    static auto warningWStr = app.i18n.GetConfigWarningText(app.config.warnings);
    static Gdiplus::Color baseBgColor = app.config.backgroundColor;

    // Create a background brush with appState.windowPage.alpha applied
    Gdiplus::SolidBrush bgBrush(ApplyAlpha(&baseBgColor, app.page.GetBackgroundAlpha()));

    Gdiplus::Graphics graphics(hdcMem);
    graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
    graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
    graphics.FillRectangle(&bgBrush, 0, 0, w, h);

    // * These functions will decide internally whether to draw based on current state
    if (!warningWStr.empty()) {
        DrawWarning(graphics, MAX_ALPHA, w, h, warningWStr);
    }

    // !
    __DrawDebug(graphics, w, h);

    index.Draw(graphics, w, h);
}

// & Here we do not use appState.screenW/H.
// Although they are equivalent, we still need to write program that has
// more compilcated logic. Consider future features like responsive layout.
SIZE Renderer::GetWH(HWND hWnd) {
    RECT rc;
    GetClientRect(hWnd, &rc);
    int w = rc.right - rc.left;
    int h = rc.bottom - rc.top;
    int buttonCount = static_cast<int>(index.home.menu.size());
    float centerIndex = (buttonCount - 1) * 0.5f;

    for (short i = 0; i < buttonCount; i++) {
        index.home.menu[i].Center(buttonCount, i, w, h);
    }
    return {w, h};
}

void Renderer::UpdateLayered(HWND hWnd) {
    static SIZE wh = GetWH(hWnd);
    HDC hdcScreen = GetDC(NULL);
    HDC hdcMem = CreateCompatibleDC(hdcScreen);

    BITMAPINFO bmi;
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = wh.cx;
    bmi.bmiHeader.biHeight = -wh.cy;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    void* pvBits = nullptr;
    HBITMAP hBitmap = CreateDIBSection(hdcScreen, &bmi, DIB_RGB_COLORS, &pvBits, NULL, 0);
    if (hBitmap == NULL) {
        MessageBoxW(nullptr, app.i18n.ErrCreateBitmap.c_str(), app.i18n.ErrTitle.c_str(),
                    MB_ICONERROR);
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);
        return;
    }

    HGDIOBJ oldBmp = SelectObject(hdcMem, hBitmap);
    DrawToMemoryDC(hdcMem, wh.cx, wh.cy);
    POINT ptWin = {0, 0};
    SIZE sizeWin = {wh.cx, wh.cy};

    // appState.page.alpha
    BLENDFUNCTION blend = {AC_SRC_OVER, 0, MAX_ALPHA, AC_SRC_ALPHA};
    UpdateLayeredWindow(hWnd, hdcScreen, &ptWin, &sizeWin, hdcMem, &ptWin, 0, &blend, ULW_ALPHA);
    SelectObject(hdcMem, oldBmp);
    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
    ReleaseDC(NULL, hdcScreen);
}
