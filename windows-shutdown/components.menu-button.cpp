#include "components.menu-button.h"

#include "resource.h"
#include "realify.h"

#include "style.menu-button.h"
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
            return IDB_DONATEPNG;  // Default to config icon
    }
}

MenuButton::MenuButton(int x, int y, Action action)
    : Div(ElementTag::Button, Gdiplus::RectF(REALIFY(x), REALIFY(y), REALIFY(MenuButtonStyle::RADIUS),
                                             REALIFY(MenuButtonStyle::RADIUS))) {
    this->png = nullptr;
    this->action = action;

    this->resId = GetResourceIdFromAction(action);
    this->png = LoadBitmapByResourceId(app.state.hInst, this->resId);

    app.event.On(EventType::MouseMove, [this]() {
        if (!this->active) {
            return;
        }

        // highlight changed
        bool before = this->hovered;
        bool after = this->MouseHit(app.state.mouseX, app.state.mouseY);
        if (before != after) {
            app.event.Emit(EventType::Redraw);
        }
    });
}

void MenuButton::Center(int buttonCount, int index, int w, int h) {
    float centerIndex = (buttonCount - 1) * 0.5f;
    int delta = static_cast<int>(MenuButtonStyle::CENTER_DIST * (index - centerIndex));

    int centerX = w / 2;
    int centerY = h / 2;
    this->rect.X = centerX + REALIFY(MenuButtonStyle::MARGIN_LEFT) + delta;
    this->rect.Y = centerY + REALIFY(MenuButtonStyle::MARGIN_TOP);
}

bool MenuButton::MouseHit(int mx, int my) {
    // x, y of the rect means center of a circle
    int x = INTIFY(this->rect.X);
    int y = INTIFY(this->rect.Y);
    int dx = mx - x;
    int dy = my - y;
    this->hovered = (dx * dx + dy * dy <= MenuButtonStyle::TRUE_R_SQUARED);
    return this->hovered;
}

void MenuButton::Draw(Gdiplus::Graphics& graphics, const DrawParams& params) {
    int x = INTIFY(this->rect.X - MenuButtonStyle::RADIUS);
    int y = INTIFY(this->rect.Y - MenuButtonStyle::RADIUS);

    __drawingalpha = params.alpha;

    auto imgAttr = ImageAttrWithAlpha(this->png, params.alpha);

    // where and what size to draw
    Gdiplus::Rect rect(x, y, MenuButtonStyle::DIAMETER, MenuButtonStyle::DIAMETER);

    // x, y, w, h cut from the source image
    // Since button images are 512x512, appState.buttons[i].png->GetWidth() is acutally 512
    graphics.DrawImage(this->png, rect, 0, 0, this->png->GetWidth(), this->png->GetHeight(),
                       Gdiplus::UnitPixel, imgAttr.get());

    // todo 按钮高光位置有异常
    // highlight
    bool isHit = this->MouseHit(app.state.mouseX, app.state.mouseY);
    if (!isHit) {
        return;
    }

    Gdiplus::Color blended(ApplyAlpha(&colors.ButtonHighlightColor, params.alpha));
    Gdiplus::SolidBrush highlightBrush(blended);
    graphics.FillEllipse(&highlightBrush, x + MenuButtonStyle::SHADOW_WIDTH,
                         y + MenuButtonStyle::SHADOW_WIDTH,
                         MenuButtonStyle::DIAMETER - MenuButtonStyle::SHADOW_WIDTH * 2,
                         MenuButtonStyle::DIAMETER - MenuButtonStyle::SHADOW_WIDTH * 2);
}
