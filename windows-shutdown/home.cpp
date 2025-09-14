#include "home.h"
#include "style.menu-button.h"
#include "style.fade.h"
#include "realify.h"

#include "components.instruction.h"
#include "components.menu-button.h"

HomeView::HomeView() : View(Page::Home) {
}

void HomeView::initMenu() {
    // Clear existing buttons
    this->menu.clear();

    // Define the actions for the buttons
    auto& actions = this->app.config.menuButtons;

    // Create and position buttons
    int buttonCount = static_cast<int>(actions.size());
    for (int i = 0; i < buttonCount; ++i) {
        this->menu.push_back(MenuButton(0, 0, actions[i]));
        this->menu[i].Center(buttonCount, i, app.state.screenW, app.state.screenH);
    }
}

void HomeView::DrawView(Gdiplus::Graphics& graphics, const DrawParams& params) {
    BYTE alpha = app.page.GetPageAlpha(this->page);
    if (!params.rect) {
        throw "rect为空";
    }

    int w = INTIFY(params.rect->Width);
    int h = INTIFY(params.rect->Height);

    // Draw image buttons (original logic)
    for (int i = 0; i < this->menu.size(); ++i) {
        auto& b = this->menu[i];
        int x = INTIFY(b.rect.X - MENU_BUTTON::R);
        int y = INTIFY(b.rect.Y - MENU_BUTTON::R);

        // where and what size to draw
        DrawParams params = {.alpha = alpha};
        b.Draw(graphics, params);
    }

    // Draw exit instruction below buttons
    static Gdiplus::RectF instrRect(
        0, (h / 2.0f) + MENU_BUTTON::R + MENU_BUTTON::MARGIN_TOP + MENU_BUTTON::MARGIN_BOTTOM, w,
        h);
    DrawInstruction(graphics, alpha, &instrRect, app.i18n.PressAnyKeyToExit);
}

void HomeView::Activate() {
    View::Activate();
    for (auto& b : this->menu) {
        b.Activate();
    }
}

void HomeView::Deactivate() {
    View::Deactivate();
    for (auto& b : this->menu) {
        b.Deactivate();
    }
}
