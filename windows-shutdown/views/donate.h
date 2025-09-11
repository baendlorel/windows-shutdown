#pragma once
#include "framework.h"
#include "view.h"

class DonateView : public View {
   private:
    void DrawView(Gdiplus::Graphics& graphics, int w, int h);
};