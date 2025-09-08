#pragma once
#include "framework.h"

// consts
#include "consts/core.h"

struct MenuButton {
   public:
    Action action;

    int x;
    int y;

    int resId;
    Gdiplus::Bitmap* png;

   public:
    MenuButton(int resId, Action action);

    void LoadPNGFromResource(HINSTANCE hInst);

    void Center(int buttonCount, int index, int w, int h);

    bool MouseHit(int mx, int my) const;
};