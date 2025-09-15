#pragma once
#include "framework.h"
#include "mini-ui.h"

class DonateView : public View {
   public:
    DonateView() : View(Page::Donate) {};

   private:
    void draw_view(Gdiplus::Graphics& graphics, const DrawParams& params) override;
};
