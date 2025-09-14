#pragma once
#include "mini-ui.core.h"
#include "mini-ui.element.h"

class View : public Element {
   protected:
    // Page identifier
    Page page = Page::None;

   public:
    View(Page page) : page(page) {};
    virtual ~View() = default;

   public:
    bool IsInvisible() const override {
        return app.page.GetPageAlpha(this->page) == 0;
    }
};