#include "render.h"
#include <format>
#include "style.font.h"
#include "realify.h"

#include "mini-ui.h"
#include "components.warning.h"

void Render::__DrawDebug(Gdiplus::Graphics& graphics, const Gdiplus::REAL w,
                         const Gdiplus::REAL h) {
    static Gdiplus::FontFamily fontFamily(app::i18n.FontFamilyName.c_str());
    static Gdiplus::SolidBrush brush(Gdiplus::Color(255, 166, 0));
    static Gdiplus::StringFormat format;
    static const Gdiplus::RectF rect(w - 900, 20, w, h);
    static const Gdiplus::Font font =
        Gdiplus::Font(&fontFamily, INSTRUCTION_FONT_SIZE, Gdiplus::FontStyleBold);
    auto pageName = [](const app::Page p) -> const wchar_t* {
        switch (p) {
            case app::Page::None:
                return L"None";
            case app::Page::Home:
                return L"Home";
            case app::Page::Countdown:
                return L"Countdown";
            case app::Page::Donate:
                return L"Donate";
        }
    };

    auto menuActive = -1;
    auto __drawingalpha = -1;

    if (this->index->home.menu.size() > 0) {
        menuActive = this->index->home.menu[0].is_active() ? 1 : 0;
        __drawingalpha = this->index->home.menu[0].__drawingalpha;
    }

    static int count = 0;
    count++;
    auto str = std::format(
        L"{}, Home:{} (menu active: {}, {}), Cnt:{}, None:{}\nCur:{}, next:{}, fading:{}", count,
        app::page.get_page_alpha(app::Page::Home), menuActive, __drawingalpha,
        app::page.get_page_alpha(app::Page::Countdown), app::page.get_page_alpha(app::Page::None),
        pageName(app::page.current), pageName(app::page.next),
        (app::page.fading ? L"true" : L"false"));

    graphics.DrawString(str.c_str(), -1, &font, rect, &format, &brush);
}

void Render::draw_to_memory_dc(const HDC hdcMem, const Gdiplus::REAL w, const Gdiplus::REAL h) {
    static auto warningWStr = app::i18n.GetConfigWarningText(app::config.warnings);
    static Gdiplus::Color baseBgColor = app::config.background_color;

    // Create a background brush with appState.windowPage.alpha applied
    Gdiplus::SolidBrush bgBrush(apply_alpha(&baseBgColor, app::page.get_background_alpha()));

    Gdiplus::Graphics graphics(hdcMem);
    graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
    graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
    graphics.FillRectangle(&bgBrush, 0, 0, to_int(w), to_int(h));

    // * These functions will decide internally whether to draw based on current state
    if (!warningWStr.empty()) {
        draw_warning(graphics, fade::MAX_ALPHA, w, h, warningWStr);
    }

    // !
    __DrawDebug(graphics, w, h);

    Gdiplus::RectF rect(0, 0, w, h);
    this->index->Draw(graphics, {.rect = &rect});
}

// Here we do not use appState.screenW/H.
// Although they are equivalent, we still need to write program that has
// more complicated logic. Consider future features like responsive layout.
SIZE Render::get_wh(const HWND hWnd) const {
    RECT rc;
    GetClientRect(hWnd, &rc);
    const int w = rc.right - rc.left;
    const int h = rc.bottom - rc.top;
    auto& menu = this->index->home.menu;
    const int buttonCount = static_cast<int>(menu.size());

    for (int i = 0; i < buttonCount; i++) {
        menu[i].center(buttonCount, i, w, h);
    }
    return {w, h};
}

void Render::update_layered(const HWND hWnd) {
    static SIZE sizeWin = get_wh(hWnd);
    const HDC hdcScreen = GetDC(nullptr);
    const HDC hdcMem = CreateCompatibleDC(hdcScreen);

    BITMAPINFO bmi;
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = sizeWin.cx;
    bmi.bmiHeader.biHeight = -sizeWin.cy;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    void* pvBits = nullptr;
    const HBITMAP hBitmap = CreateDIBSection(hdcScreen, &bmi, DIB_RGB_COLORS, &pvBits, nullptr, 0);
    if (hBitmap == nullptr) {
        MessageBoxW(nullptr, app::i18n.ErrCreateBitmap.c_str(), app::i18n.ErrTitle.c_str(),
                    MB_ICONERROR);
        DeleteDC(hdcMem);
        ReleaseDC(nullptr, hdcScreen);
        return;
    }

    const HGDIOBJ oldBmp = SelectObject(hdcMem, hBitmap);
    draw_to_memory_dc(hdcMem, to_real(sizeWin.cx), to_real(sizeWin.cy));
    POINT ptWin = {0, 0};

    BLENDFUNCTION blend = {AC_SRC_OVER, 0, fade::MAX_ALPHA, AC_SRC_ALPHA};
    UpdateLayeredWindow(hWnd, hdcScreen, &ptWin, &sizeWin, hdcMem, &ptWin, 0, &blend, ULW_ALPHA);
    SelectObject(hdcMem, oldBmp);
    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
    ReleaseDC(nullptr, hdcScreen);

    app::state.need_redraw = false;
}
