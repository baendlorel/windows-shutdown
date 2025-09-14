#include "home.h"

#include <stdexcept>

#include "style.menu-button.h"
#include "realify.h"

#include "components.instruction.h"
#include "components.menu-button.h"

HomeView::HomeView() : View(Page::Home) {
}

void HomeView::init_menu() {
    // Clear existing buttons
    this->menu.clear();

    // Define the actions for the buttons
    auto& actions = this->app_.config.menuButtons;

    // Create and position buttons
    const int button_count = static_cast<int>(actions.size());
    for (int i = 0; i < button_count; ++i) {
        this->menu.emplace_back(0, 0, actions[i]);
        this->menu[i].center(button_count, i, app_.state.screenW, app_.state.screenH);
    }
}

void HomeView::DrawView(Gdiplus::Graphics& graphics, const DrawParams& params) {
    const BYTE alpha = app_.page.GetPageAlpha(this->page_);
    if (!params.rect) {
        throw std::runtime_error("rect为空");
    }

    const Gdiplus::REAL w = params.rect->Width;
    const Gdiplus::REAL h = params.rect->Height;

    // Draw image buttons (original logic)
    for (auto& b : this->menu) {
        int x = INTIFY(b.rect.X - MenuButtonStyle::RADIUS);
        int y = INTIFY(b.rect.Y - MenuButtonStyle::RADIUS);

        // where and what size to draw
        b.draw(graphics, {.alpha = alpha});
    }

    // Draw exit instruction below buttons
    static Gdiplus::RectF instr_rect(0,
                                     h * 0.5f + MenuButtonStyle::RADIUS +
                                         MenuButtonStyle::MARGIN_TOP +
                                         MenuButtonStyle::MARGIN_BOTTOM,
                                     w, h);
    DrawInstruction(graphics, alpha, &instr_rect, app_.i18n.PressAnyKeyToExit);
}

void HomeView::activate() {
    View::activate();
    for (auto& b : this->menu) {
        b.activate();
    }
}

void HomeView::deactivate() {
    View::deactivate();
    for (auto& b : this->menu) {
        b.deactivate();
    }
}
