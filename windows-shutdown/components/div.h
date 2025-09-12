// Base UI container for shared data
#pragma once
#include <functional>
#include "framework.h"
#include "consts/core.h"
#include "consts/color-set.h"
#include "app-state.h"
#include "i18n.h"
#include "ui.h"

struct DrawParams {
    unsigned char alpha;

    // optional, for text drawing or image drawing
    Gdiplus::RectF* rect = nullptr;
    DrawTextParams* textParams = nullptr;
};

class Div {
   protected:
    AppState& appState = AppState::GetInstance();
    I18N& i18n = I18N::GetInstance();
    ColorSet& colors = ColorSet::GetInstance();

    // when alpha is MAX_ALPHA, active state is true
    bool active = false;

   public:
    Div(ElementTag tag, const Gdiplus::RectF& rect) : tag(tag), rect(rect) {};
    virtual ~Div() = default;

   private:
    std::function<void(HWND)> onClickCallback;

   public:
    ElementTag tag;
    Gdiplus::RectF rect;  // Position and size

    virtual bool MouseHit(int mx, int my) const;
    virtual void Draw(Gdiplus::Graphics& graphics, DrawParams& params) = 0;

    void OnClick(std::function<void(HWND)> cb) {
        this->onClickCallback = std::move(cb);
    }

    void TriggerClick(HWND hwnd) {
        if (this->onClickCallback && this->active) {
            this->onClickCallback(hwnd);
        }
    }

    void Activate() {
        this->active = true;
    }

    void Deactivate() {
        this->active = false;
    }

    bool IsActive() const {
        return this->active;
    }
};
