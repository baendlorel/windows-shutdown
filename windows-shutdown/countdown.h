#pragma once
#include "framework.h"
#include "mini-ui.h"

class CountdownView : public View {
   public:
    CountdownView() : View(Page::Countdown) {};

   private:
    void draw_view(Gdiplus::Graphics& graphics, const DrawParams& params) override;
};
