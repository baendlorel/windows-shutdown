#pragma once
#include "framework.h"

// consts
#include "consts/button-style.h"
#include "consts/state.h"

struct ImageButton {
   public:
    Button kind;

    int x;
    int y;

    int resId;
    Gdiplus::Bitmap* png;

   public:
    ImageButton(int resId, Button kind);

    void LoadPNGFromResource(HINSTANCE hInst);

    void Center(int marginLeft, int marginTop, int w, int h, int index);

    bool MouseHit(int mx, int my) const;
};