#include "components.menu-button.h"

#include "resource.h"
#include "realify.h"

#include "style.menu-button.h"
#include "bitmap-loader.h"

// Helper function to get resource ID from Action
namespace {
int get_resource_id_from_action(const app::Action action) {
    switch (action) {
        case app::Action::Donate:
            return IDB_DONATEPNG;
        case app::Action::Config:
            return IDB_CONFIGPNG;
        case app::Action::Lock:
            return IDB_LOCKPNG;
        case app::Action::Sleep:
            return IDB_SLEEPPNG;
        case app::Action::Restart:
            return IDB_RESTARTPNG;
        case app::Action::Shutdown:
            return IDB_SHUTDOWNPNG;
        case app::Action::None:
            return NULL;
    }
    return NULL;
}
}  // namespace

MenuButton::MenuButton(const int x, const int y, const app::Action action)
    : Div(app::ElementTag::Button,
          Gdiplus::RectF(to_real(x), to_real(y), to_real(MenuButtonStyle::RADIUS),
                         to_real(MenuButtonStyle::RADIUS))) {
    this->png = nullptr;
    this->action = action;

    this->res_id = get_resource_id_from_action(action);
    this->png = load_bitmap_by_resource_id(app.state.hInst, this->res_id);

    app.event.On(EventType::MouseMove, [this]() {
        if (!this->active_) {
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

void MenuButton::center(const int button_count, const int index, const int w, const int h) {
    const float trueIndex = to_real(index) - to_real(button_count - 1) * 0.5f;
    const float delta = MenuButtonStyle::CENTER_DIST * trueIndex;

    const float centerX = to_real(w) / 2.0f;
    const float centerY = to_real(h) / 2.0f;

    this->rect.X = centerX + to_real(MenuButtonStyle::MARGIN_LEFT) + delta;
    this->rect.Y = centerY + to_real(MenuButtonStyle::MARGIN_TOP);
}

bool MenuButton::MouseHit(const int mx, const int my) {
    // x, y of the rect means center of a circle
    const int x = to_int(this->rect.X);
    const int y = to_int(this->rect.Y);
    const int dx = mx - x;
    const int dy = my - y;
    this->hovered = (dx * dx + dy * dy <= MenuButtonStyle::TRUE_R_SQUARED);
    return this->hovered;
}

void MenuButton::draw(Gdiplus::Graphics& graphics, const DrawParams& params) {
    const int x = to_int(this->rect.X - MenuButtonStyle::RADIUS);
    const int y = to_int(this->rect.Y - MenuButtonStyle::RADIUS);

    __drawingalpha = params.alpha;

    const auto imgAttr = painter::image_attr_with_alpha(this->png, params.alpha);

    // where and what size to draw
    const Gdiplus::Rect rect(x, y, MenuButtonStyle::DIAMETER, MenuButtonStyle::DIAMETER);

    // x, y, w, h cut from the source image
    // Since button images are 512x512, appState.buttons[i].png->GetWidth() is actually 512
    const int w = static_cast<int>(this->png->GetWidth());
    const int h = static_cast<int>(this->png->GetHeight());
    graphics.DrawImage(this->png, rect, 0, 0, w, h, Gdiplus::UnitPixel, imgAttr.get());

    // todo 按钮高光位置有异常
    // highlight
    if (const bool isHit = this->MouseHit(app.state.mouseX, app.state.mouseY); !isHit) {
        return;
    }

    const Gdiplus::Color blended(painter::apply_alpha(&colors.button_highlight, params.alpha));
    const Gdiplus::SolidBrush highlightBrush(blended);
    graphics.FillEllipse(&highlightBrush, x + MenuButtonStyle::SHADOW_WIDTH,
                         y + MenuButtonStyle::SHADOW_WIDTH,
                         MenuButtonStyle::DIAMETER - MenuButtonStyle::SHADOW_WIDTH * 2,
                         MenuButtonStyle::DIAMETER - MenuButtonStyle::SHADOW_WIDTH * 2);
}
