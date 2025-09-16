#pragma once
#include "event.h"
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

    Event event_;

    std::vector<Element> children_;

   public:
    virtual void activate() {
        this->active_ = true;
        for (auto& c : this->children_) {
            c.activate();
        }
    }

    virtual void deactivate() {
        this->active_ = false;
        for (auto& c : this->children_) {
            c.deactivate();
        }
    }

    [[nodiscard]] bool is_active() const {
        return this->active_;
    }

    // normally use page alpha to determine visibility
    // for divs, we can always return false
    [[nodiscard]] virtual bool is_invisible() const {
        return false;
    }

    virtual void on(const app::EventType evt, const std::function<void()>& handler) {
        this->event_.on(evt, handler);
    }

    virtual void emit(const app::EventType evt) {
        this->event_.emit(evt);
        for (auto& c : this->children_) {
            c.emit(evt);
        }
    }

    virtual void draw(Gdiplus::Graphics& graphics, const DrawParams& params) = 0;
};
