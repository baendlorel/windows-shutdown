#pragma once
#include "framework.h"
#include "view.h"

class HomeView : public View {
   private:
    void DrawView(Gdiplus::Graphics& graphics, int w, int h);
};