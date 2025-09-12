#pragma once
#include "framework.h"
#include "view.h"

class CountdownView : public View {
   public:
    CountdownView() : View(Page::Countdown) {};

   private:
    void DrawView(Gdiplus::Graphics& graphics, int w, int h);
};
