#pragma once
#include "framework.h"
#include "mini-ui.h"
#include "components.menu-button.h"

class HomeView : public View {
   public:
    HomeView();

    void initMenu();
    std::vector<MenuButton> menu;

    void Activate();
    void Deactivate();

   private:
    void DrawView(Gdiplus::Graphics& graphics, DrawParams& params) override;
};
