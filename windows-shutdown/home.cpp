#include "home.h"

#include <stdexcept>

#include "style.menu-button.h"

#include "components.instruction.h"
#include "components.menu-button.h"

void HomeView::init_menu() {
    // Clear existing buttons
    this->menu.clear();

    // Define the actions for the buttons
    auto& actions = app::config.menu_buttons;

    // Create and position buttons
    const int button_count = static_cast<int>(actions.size());

    // TODO 当跟随element的事件触发机制完成，这里将不会受到扩容影响
    // this->menu.reserve(actions.size());
    for (int i = 0; i < button_count; ++i) {
        this->menu.emplace_back(0, 0, actions[i]);
        this->menu[i].center(button_count, i, app::state.screen_w, app::state.screen_h);
    }
}

void HomeView::draw(Gdiplus::Graphics& graphics, const DrawParams& params) {
    const BYTE alpha = app::page.get_page_alpha(this->page_);
    if (!params.rect) {
        throw std::runtime_error("rect为空");
    }

    const Gdiplus::REAL w = params.rect->Width;
    const Gdiplus::REAL h = params.rect->Height;

    // Draw image buttons (original logic)
    for (auto& b : this->menu) {
        // int x = to_int(b.rect.X - menu_button_style::RADIUS);
        // int y = to_int(b.rect.Y - menu_button_style::RADIUS);

        // where and what size to draw
        b.draw(graphics, {.alpha = alpha});
    }

    // Draw exit instruction below buttons
    static Gdiplus::RectF instr_rect(0,
                                     h * 0.5f + menu_button_style::RADIUS +
                                         menu_button_style::MARGIN_TOP +
                                         menu_button_style::MARGIN_BOTTOM,
                                     w, h);
    draw_instruction(graphics, alpha, &instr_rect, app::i18n.PressAnyKeyToExit);
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
