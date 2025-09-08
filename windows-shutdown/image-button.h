#pragma once
#include "framework.h"

// consts
#include "consts/core.h"

struct ImageButton {
   public:
    Action action;

    int x;
    int y;

    int resId;
    Gdiplus::Bitmap* png;

   public:
    ImageButton(int resId, Action action);

    void LoadPNGFromResource(HINSTANCE hInst);

    void Center(int buttonCount, int index, int w, int h);

    bool MouseHit(int mx, int my) const;
};