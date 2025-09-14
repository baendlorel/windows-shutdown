#pragma once
#include "framework.h"
#include "mini-ui.h"

class CountdownView : public View {
   public:
    CountdownView() : View(Page::Countdown) {};

   private:
    void DrawView(Gdiplus::Graphics& graphics, const DrawParams& params) override;
};
