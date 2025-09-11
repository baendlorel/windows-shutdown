// Base UI container for shared data
#pragma once
#include "consts/core.h"
#include "framework.h"
#include "consts/color-set.h"
#include "app-state.h"
#include "i18n.h"
#include <functional>

class View;
class Div {
   protected:
    AppState& appState = AppState::GetInstance();
    I18N& i18n = I18N::GetInstance();
    ColorSet& colors = ColorSet::GetInstance();

   public:
    Div(ElementTag tag, const Gdiplus::RectF& rect) : tag(tag), rect(rect) {};
    virtual ~Div() = default;

   public:
    ElementTag tag;
    Gdiplus::RectF rect;  // Position and size
    std::function<void(HWND)> onClickCallback;

    virtual bool MouseHit(int mx, int my) const;

    void OnClick(std::function<void(HWND)> cb) {
        this->onClickCallback = std::move(cb);
    }
};
