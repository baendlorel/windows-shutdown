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

void MenuButton::LoadPNGFromResource(HINSTANCE hInst) {
    auto& i18n = I18N::GetInstance();
    this->png = LoadBitmapByResourceId(hInst, this->resId);
}

MenuButton::MenuButton(int x, int y, Action action)
    : Div(ElementTag::Button, Gdiplus::RectF(x, y, BUTTON_RADIUS, BUTTON_RADIUS)) {
    this->png = nullptr;
    this->action = action;

    this->resId = GetResourceIdFromAction(action);
    LoadPNGFromResource(appState.hInst);
}

void MenuButton::Center(int buttonCount, int index, int w, int h) {
    float centerIndex = (buttonCount - 1) * 0.5f;
    int delta = static_cast<int>(BUTTON_CENTER_DISTANCE * (index - centerIndex));

    int centerX = w / 2;
    int centerY = h / 2;
    this->rect.X = centerX + BUTTON_MARGIN_LEFT + delta;
    this->rect.Y = centerY + BUTTON_MARGIN_TOP;
}

bool MenuButton::MouseHit(int mx, int my) const {
    int x = this->rect.X + BUTTON_RADIUS;
    int y = this->rect.Y + BUTTON_RADIUS;
    int dx = mx - x;
    int dy = my - y;
    return (dx * dx + dy * dy <= BUTTON_TRUE_RADIUS_SQUARED);
}

void MenuButton::Draw(Gdiplus::Graphics& graphics, DrawParams& params) {
    int x = this->rect.X - BUTTON_RADIUS;
    int y = this->rect.Y - BUTTON_RADIUS;

    auto imgAttr = ImageAttrWithAlpha(this->png, params.alpha);

    // where and what size to draw
    Gdiplus::Rect rect(x, y, BUTTON_DIAMETER, BUTTON_DIAMETER);

    // x, y, w, h cut from the source image
    // Since button images are 512x512, appState.buttons[i].png->GetWidth() is acutally 512
    graphics.DrawImage(this->png, rect, 0, 0, this->png->GetWidth(), this->png->GetHeight(),
                       Gdiplus::UnitPixel, imgAttr.get());

    // highlight
    bool isHit = this->MouseHit(appState.mouseX, appState.mouseY);
    if (!isHit) {
        return;
    }

    Gdiplus::Color blended(ApplyAlpha(&colors.ButtonHighlightColor, params.alpha));
    Gdiplus::SolidBrush highlightBrush(blended);
    graphics.FillEllipse(&highlightBrush, x + BUTTON_SHADOW_WIDTH, y + BUTTON_SHADOW_WIDTH,
                         BUTTON_DIAMETER - BUTTON_SHADOW_WIDTH * 2,
                         BUTTON_DIAMETER - BUTTON_SHADOW_WIDTH * 2);
}
