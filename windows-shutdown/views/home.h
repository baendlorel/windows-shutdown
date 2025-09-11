#pragma once
#include "framework.h"
#include "view.h"

class HomeView : View {
   public:
    void DrawView(Gdiplus::Graphics& graphics, int w, int h);
};