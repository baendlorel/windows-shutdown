#pragma once
#include "utils.class.h"
#include "mini-ui.core.h"
#include "mini-ui.element.h"

class View : public Element {
    NO_COPY_DEFAULT_MOVE(View)

   protected:
    // Page identifier
    app::Page page_ = app::Page::None;

   public:
    explicit View(const app::Page page) : page_(page) {
    }

    ~View() override = default;

   public:
    [[nodiscard]] bool is_invisible() const override {
        return app::page.get_page_alpha(this->page_) == 0;
    }
};