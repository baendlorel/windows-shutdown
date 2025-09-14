#pragma once
#include "mini-ui.h"
#include "consts.app.h"

class MenuButton : public Div {
   public:
    Action action;
    int resId;
    Gdiplus::Bitmap* png;

    BYTE __drawingalpha = 0;

   public:
    MenuButton(int x, int y, Action action);

   public:
    bool MouseHit(int mx, int my);
    void Draw(Gdiplus::Graphics& graphics, DrawParams& params);

    // menu buttons are centered horizontally
    void Center(int buttonCount, int index, int w, int h);
};