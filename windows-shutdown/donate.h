#pragma once
#include "framework.h"
#include "mini-ui.h"

class DonateView : public View {
   public:
    DonateView() : View(Page::Donate) {};

   private:
    void DrawView(Gdiplus::Graphics& graphics, int w, int h);
};