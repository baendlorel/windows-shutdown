#pragma once
#include "mini-ui.core.h"
#include "mini-ui.element.h"

class Div : public Element {
   protected:
    App& app = App::GetInstance();
    ColorSet& colors = ColorSet::GetInstance();

   public:
    Div(ElementTag tag, const Gdiplus::RectF& rect) : tag(tag), rect(rect) {};
    virtual ~Div() = default;

   private:
    std::function<void(HWND)> onClickCallback;

   public:
    ElementTag tag;
    Gdiplus::RectF rect;  // Position and size
    bool hovered = false;

    virtual bool MouseHit(int mx, int my) {
        this->hovered =
            mx >= rect.X && mx <= rect.Y && my >= rect.GetTop() && my <= rect.GetBottom();
        return this->hovered;
    }

    void OnClick(std::function<void(HWND)> cb) {
        this->onClickCallback = std::move(cb);
    }

    void TriggerClick(HWND hwnd) {
        if (this->onClickCallback && this->active_) {
            this->onClickCallback(hwnd);
        }
    }
};