#pragma once
#include "mini-ui.core.h"
#include "mini-ui.element.h"

class View : public Element {
   protected:
    // Page identifier
    Page page_ = Page::None;

   public:
    View(const Page page) : page_(page) {
    }

    virtual ~View() = default;

   public:
    bool is_invisible() const override {
        return app_.page.GetPageAlpha(this->page_) == 0;
    }
};