#pragma once
#include "framework.h"
#include "consts.h"

struct ImageButton {
    int x = 0;
    int y = 0;
    int r = BUTTON_RADIUS;
    const wchar_t* label = L"";

    int resId = 0;
    Gdiplus::Bitmap* png = nullptr;

    void LoadPNGFromResource(HINSTANCE hInst);

    void Center(int w, int h, int index);

    bool mouseHit(int mx, int my) const {
        int dx = mx - x;
        int dy = my - y;
        return (dx * dx + dy * dy <= r * r);
    }
};