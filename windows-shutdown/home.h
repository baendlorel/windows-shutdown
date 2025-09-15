#pragma once
#include "framework.h"
#include "mini-ui.h"
#include "components.menu-button.h"

class HomeView : public View {
   public:
    HomeView() : View(app::Page::Home) {
    }

    void init_menu();
    std::vector<MenuButton> menu;

    void activate() override;
    void deactivate() override;

   private:
    void draw(Gdiplus::Graphics& graphics, const DrawParams& params) override;
};
