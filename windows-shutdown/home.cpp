#include "home.h"
#include "style.button.h"
#include "style.fade.h"
#include "app.state.h"
#include "i18n.h"

#include "components.instruction.h"
#include "components.menu-button.h"

void HomeView::initMenu() {
    // Clear existing buttons
    this->menu.clear();

    // Define the actions for the buttons
    auto& actions = this->appState.config.menuButtons;

    // Create and position buttons
    int buttonCount = static_cast<int>(actions.size());
    for (int i = 0; i < buttonCount; ++i) {
        this->menu.push_back(MenuButton(0, 0, actions[i]));
        this->menu[i].Center(buttonCount, i, appState.screenW, appState.screenH);
    }
}

void HomeView::DrawView(Gdiplus::Graphics& graphics, int w, int h) {
    BYTE alpha = appPage.GetPageAlpha(this->page);

    // Draw image buttons (original logic)
    for (int i = 0; i < this->menu.size(); ++i) {
        auto& b = this->menu[i];
        int x = b.rect.X - BUTTON_RADIUS;
        int y = b.rect.Y - BUTTON_RADIUS;

        // where and what size to draw
        DrawParams params = {.alpha = alpha};
        b.Draw(graphics, params);
    }

    // Draw exit instruction below buttons
    static Gdiplus::RectF instrRect(
        0, (h / 2.0f) + BUTTON_RADIUS + BUTTON_MARGIN_TOP + BUTTON_MARGIN_BOTTOM, w, h);
    DrawInstruction(graphics, alpha, &instrRect, i18n.PressAnyKeyToExit);
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
