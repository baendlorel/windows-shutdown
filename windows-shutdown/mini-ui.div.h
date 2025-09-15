#pragma once
#include "utils.class.h"
#include "mini-ui.core.h"
#include "mini-ui.element.h"
#include "realify.h"

class Div : public Element {
    NO_COPY_DEFAULT_MOVE(Div)

   public:
    Div(const app::ElementTag tag, const Gdiplus::RectF& rect) : tag(tag), rect(rect) {
    }

    ~Div() override = default;

   private:
    std::function<void(HWND)> on_click_callback_;

   public:
    app::ElementTag tag;
    Gdiplus::RectF rect;  // Position and size
    bool hovered = false;

    virtual bool mouse_hit(const int mx, const int my) {
        const float rx = to_real(mx);
        const float ry = to_real(my);
        this->hovered =
            rx >= rect.X && rx <= rect.Y && ry >= rect.GetTop() && ry <= rect.GetBottom();
        return this->hovered;
    }

    void on_click(std::function<void(HWND)> cb) {
        this->on_click_callback_ = std::move(cb);
    }

    void trigger_click(const HWND hwnd) const {
        if (this->on_click_callback_ && this->active_) {
            this->on_click_callback_(hwnd);
        }
    }
};