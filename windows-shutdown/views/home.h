#pragma once
#include "framework.h"
#include "view.h"

class HomeView : public View {
   public:
    void initMenu();

   private:
    std::vector<MenuButton> buttons;
    void DrawView(Gdiplus::Graphics& graphics, int w, int h);
};