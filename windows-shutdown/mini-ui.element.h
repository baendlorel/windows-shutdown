#pragma once
#include "mini-ui.core.h"

class Element {
   public:
    Element() = default;

    Element(const Element&) = delete;
    Element& operator=(const Element&) = delete;
    Element(Element&&) noexcept = default;
    Element& operator=(Element&&) noexcept = default;
    virtual ~Element() = default;

   protected:
    App* app_ = App::GetInstance();
    color_set* colors_ = color_set::GetInstance();

    // when alpha is MAX_ALPHA, active state is true
    bool active_ = false;

   public:
    virtual void activate() {
        this->active_ = true;
    }

    virtual void deactivate() {
        this->active_ = false;
    }

    [[nodiscard]] bool is_active() const {
        return this->active_;
    }

    // normally use page alpha to determine visibility
    // for divs, we can always return false
    [[nodiscard]] virtual bool is_invisible() const {
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

        this->draw_view(graphics, params);
    }

   protected:
    virtual void draw_view(Gdiplus::Graphics& graphics, const DrawParams& params) = 0;
};
