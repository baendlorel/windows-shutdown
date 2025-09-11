#pragma once
#include <functional>
#include "div.h"
#include "consts/core.h"

class MenuButton : public Div {
   public:
    Action action;
    int resId;
    Gdiplus::Bitmap* png;

   public:
    MenuButton(int x, int y, Action action);
    void Center(int buttonCount, int index, int w, int h);
    bool MouseHit(int mx, int my) const;

   private:
    void LoadPNGFromResource(HINSTANCE hInst);
};