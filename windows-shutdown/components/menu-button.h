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
    bool MouseHit(int mx, int my) const;
    void Draw(Gdiplus::Graphics& graphics, DrawParams& params);
    void DrawHighlight(Gdiplus::Graphics& graphics, DrawParams& params);

    void Center(int buttonCount, int index, int w, int h);

   private:
    void LoadPNGFromResource(HINSTANCE hInst);
};