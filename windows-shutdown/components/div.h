// Base UI container for shared data
#pragma once
#include "framework.h"
#include <functional>

class Div {
   private:
    Gdiplus::RectF rect;  // Position and size

   public:
    bool visible = true;  // Visibility
    std::function<void(HWND)> onClickCallback;

    Div(const Gdiplus::RectF& rect, bool visible = true) : rect(rect), visible(visible) {};
    virtual ~Div() = default;

    bool MouseHitRect(int mx, int my) const;

    bool MouseHitCircle(int mx, int my) const;

    void OnClick(std::function<void(HWND)> cb) {
        this->onClickCallback = std::move(cb);
    }
};
