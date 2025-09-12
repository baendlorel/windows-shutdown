#pragma once
#include "framework.h"
#include "view.h"
#include "components/menu-button.h"

class HomeView : public View {
   public:
    HomeView() : View(Page::Home) {};

    void initMenu();
    std::vector<MenuButton> menu;

    void Activate();
    void Deactivate();

   private:
    void DrawView(Gdiplus::Graphics& graphics, int w, int h);
};