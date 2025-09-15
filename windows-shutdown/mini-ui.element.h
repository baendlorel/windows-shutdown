#pragma once
#include "utils.class.h"
#include "mini-ui.core.h"

class Element {
    NO_COPY_DEFAULT_MOVE(Element)

   public:
    Element() = default;

    virtual ~Element() = default;

   protected:
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

    virtual void draw(Gdiplus::Graphics& graphics, const DrawParams& params) = 0;
};
