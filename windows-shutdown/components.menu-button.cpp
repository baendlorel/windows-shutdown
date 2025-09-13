#include "components.menu-button.h"

#include "resource.h"
#include "realify.h"

#include "style.button.h"
#include "bitmap-loader.h"

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
    this->png = LoadBitmapByResourceId(hInst, this->resId);
}

MenuButton::MenuButton(int x, int y, Action action)
    : Div(ElementTag::Button,
          Gdiplus::RectF(REALIFY(x), REALIFY(y), REALIFY(BUTTON_RADIUS), REALIFY(BUTTON_RADIUS))) {
    this->png = nullptr;
    this->action = action;

    this->resId = GetResourceIdFromAction(action);
    LoadPNGFromResource(app.state.hInst);
}

void MenuButton::Center(int buttonCount, int index, int w, int h) {
    float centerIndex = (buttonCount - 1) * 0.5f;
    int delta = static_cast<int>(BUTTON_CENTER_DISTANCE * (index - centerIndex));

    int centerX = w / 2;
    int centerY = h / 2;
    this->rect.X = centerX + REALIFY(BUTTON_MARGIN_LEFT) + delta;
    this->rect.Y = centerY + REALIFY(BUTTON_MARGIN_TOP);
}

bool MenuButton::MouseHit(int mx, int my) const {
    int x = INTIFY(this->rect.X + BUTTON_RADIUS);
    int y = INTIFY(this->rect.Y + BUTTON_RADIUS);
    int dx = mx - x;
    int dy = my - y;
    return (dx * dx + dy * dy <= BUTTON_TRUE_RADIUS_SQUARED);
}

void MenuButton::Draw(Gdiplus::Graphics& graphics, DrawParams& params) {
    int x = INTIFY(this->rect.X - BUTTON_RADIUS);
    int y = INTIFY(this->rect.Y - BUTTON_RADIUS);

    __drawingalpha = params.alpha;

    auto imgAttr = ImageAttrWithAlpha(this->png, params.alpha);

    // where and what size to draw
    Gdiplus::Rect rect(x, y, BUTTON_DIAMETER, BUTTON_DIAMETER);

    // x, y, w, h cut from the source image
    // Since button images are 512x512, appState.buttons[i].png->GetWidth() is acutally 512
    graphics.DrawImage(this->png, rect, 0, 0, this->png->GetWidth(), this->png->GetHeight(),
                       Gdiplus::UnitPixel, imgAttr.get());

    // highlight
    bool isHit = this->MouseHit(app.state.mouseX, app.state.mouseY);
    if (!isHit) {
        return;
    }

    Gdiplus::Color blended(ApplyAlpha(&colors.ButtonHighlightColor, params.alpha));
    Gdiplus::SolidBrush highlightBrush(blended);
    graphics.FillEllipse(&highlightBrush, x + BUTTON_SHADOW_WIDTH, y + BUTTON_SHADOW_WIDTH,
                         BUTTON_DIAMETER - BUTTON_SHADOW_WIDTH * 2,
                         BUTTON_DIAMETER - BUTTON_SHADOW_WIDTH * 2);
}
