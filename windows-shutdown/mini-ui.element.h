#pragma once
#include "mini-ui.core.h"

class Element {
   public:
    Element() = default;

   protected:
    App& app_ = App::GetInstance();
    ColorSet& colors_ = ColorSet::GetInstance();

    // when alpha is MAX_ALPHA, active state is true
    bool active_ = false;

   public:
    virtual void activate() {
        this->active_ = true;
    }

    virtual void deactivate() {
        this->active_ = false;
    }

    bool is_active() const {
        return this->active_;
    }

    // normally use page alpha to determine visibility
    // for divs, we can always return false
    virtual bool is_invisible() const {
        return false;
    }

    virtual void draw(Gdiplus::Graphics& graphics, const DrawParams& params) {
        if (this->is_invisible()) {
            this->deactivate();
            return;
        }

        if (app_.page.fading) {
            this->deactivate();
        } else {
            this->activate();
        }

        this->DrawView(graphics, params);
    }

   protected:
    virtual void DrawView(Gdiplus::Graphics& graphics, const DrawParams& params) = 0;
};
