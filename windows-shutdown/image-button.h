#pragma once
#include "framework.h"
#include "consts.h"

struct ImageButton {
    int x = 0;
    int y = 0;

    int resId = 0;
    Gdiplus::Bitmap* png = nullptr;

    void LoadPNGFromResource(HINSTANCE hInst);

    void Center(int marginLeft, int marginTop, int w, int h, int index);

    bool MouseHit(int mx, int my) const;
};