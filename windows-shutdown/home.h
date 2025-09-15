#pragma once
#include "framework.h"
#include "mini-ui.h"
#include "components.menu-button.h"

class HomeView : public View {
   public:
    HomeView();

    void init_menu();
    std::vector<MenuButton> menu;

    void activate() override;
    void deactivate() override;

   private:
    void draw_view(Gdiplus::Graphics& graphics, const DrawParams& params) override;
};
