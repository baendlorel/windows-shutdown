#include "resource.h"
#include "consts/button-style.h"
#include "bitmap-loader.h"
#include "menu-button.h"
#include "i18n.h"

// Helper function to get resource ID from Action
int GetResourceIdFromAction(Action action) {
    switch (action) {
        case Action::Donate:
            return IDB_DONATEPNG;
        case Action::Config:
            return IDB_CONFIGPNG;
        case Action::Lock:
            return IDB_LOCKPNG;
        case Action::Sleep:
            return IDB_SLEEPPNG;
        case Action::Restart:
            return IDB_RESTARTPNG;
        case Action::Shutdown:
            return IDB_SHUTDOWNPNG;
        default:
            return IDB_CONFIGPNG;  // Default to config icon
    }
}

MenuButton::MenuButton(Action action) {
    this->x = 0;
    this->y = 0;
    this->png = nullptr;
    this->action = action;
    this->resId = GetResourceIdFromAction(action);
}

void MenuButton::LoadPNGFromResource(HINSTANCE hInst) {
    auto& i18n = I18N::GetInstance();
    this->png = LoadBitmapByResourceId(hInst, this->resId);
}

void MenuButton::Center(int buttonCount, int index, int w, int h) {
    float centerIndex = (buttonCount - 1) * 0.5f;
    int delta = static_cast<int>(BUTTON_CENTER_DISTANCE * (index - centerIndex));

    int centerX = w / 2;
    int centerY = h / 2;
    this->x = centerX + BUTTON_MARGIN_LEFT + delta;
    this->y = centerY + BUTTON_MARGIN_TOP;
}

bool MenuButton::MouseHit(int mx, int my) const {
    // Account for the same margins used when rendering the button so that
    // hit testing uses the visual center rather than the stored center.
    int dx = mx - this->x;
    int dy = my - this->y;
    return (dx * dx + dy * dy <= BUTTON_TRUE_RADIUS_SQUARED);
}
