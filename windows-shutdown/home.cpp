#include "home.h"

#include <stdexcept>

#include "style.menu-button.h"
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
    const int button_count = static_cast<int>(actions.size());
    for (int i = 0; i < button_count; ++i) {
        this->menu.emplace_back(0, 0, actions[i]);
        this->menu[i].Center(button_count, i, app.state.screenW, app.state.screenH);
    }
}

void HomeView::DrawView(Gdiplus::Graphics& graphics, const DrawParams& params) {
    const BYTE alpha = app.page.GetPageAlpha(this->page);
    if (!params.rect) {
        throw std::runtime_error("rect为空");
    }

    int w = INTIFY(params.rect->Width);
    int h = INTIFY(params.rect->Height);

    // Draw image buttons (original logic)
    for (auto& b : this->menu) {
        int x = INTIFY(b.rect.X - MenuButtonStyle::RADIUS);
        int y = INTIFY(b.rect.Y - MenuButtonStyle::RADIUS);

        // where and what size to draw
        b.Draw(graphics, {.alpha = alpha});
    }

    // Draw exit instruction below buttons
    static Gdiplus::RectF instrRect(0,
                                    (h / 2.0f) + MenuButtonStyle::RADIUS +
                                        MenuButtonStyle::MARGIN_TOP +
                                        MenuButtonStyle::MARGIN_BOTTOM,
                                    w, h);
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
