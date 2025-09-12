#include "home.h"
#include "components/instruction.h"
#include "consts/button-style.h"

#include "consts/effects.h"
#include "consts/color-set.h"
#include "app-state.h"
#include "i18n.h"
#include "ui.h"

void HomeView::initMenu() {
    // Clear existing buttons
    this->buttons.clear();

    // Define the actions for the buttons
    auto& actions = this->appState.config.menuButtons;

    // Create and position buttons
    int buttonCount = static_cast<int>(actions.size());
    for (int i = 0; i < buttonCount; ++i) {
        this->buttons.push_back(MenuButton(0, 0, actions[i]));
        this->buttons[i].Center(buttonCount, i, appState.screenW, appState.screenH);
    }
}

void HomeView::DrawView(Gdiplus::Graphics& graphics, int w, int h) {
    BYTE alpha = appState.page.GetPageAlpha(Page::Home);

    // Draw image buttons (original logic)
    for (int i = 0; i < this->buttons.size(); ++i) {
        auto& b = this->buttons[i];
        int x = b.rect.X - BUTTON_RADIUS;
        int y = b.rect.Y - BUTTON_RADIUS;

        // where and what size to draw
        DrawParams params = {.alpha = alpha};
        b.Draw(graphics, params);

        // Draw highlight if mouse is over the button
        bool isHit = b.MouseHit(appState.mouseX, appState.mouseY);
        if (isHit) {
            DrawParams highlightParams = {.alpha = MAX_ALPHA};
            b.DrawHighlight(graphics, highlightParams);
        }
    }

    // Draw exit instruction below buttons
    int instrY = (h / 2) + BUTTON_RADIUS + BUTTON_MARGIN_TOP + BUTTON_MARGIN_BOTTOM;
    Gdiplus::RectF instrRect(0, instrY, w, h);
    DrawInstruction(graphics, alpha, &instrRect, i18n.PressAnyKeyToExit);
}
