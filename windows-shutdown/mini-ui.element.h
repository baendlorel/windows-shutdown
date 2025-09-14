#pragma once
#include "mini-ui.core.h"

class Element {
   public:
    Element() = default;

   protected:
    App& app = App::GetInstance();
    ColorSet& colors = ColorSet::GetInstance();

    // when alpha is MAX_ALPHA, active state is true
    bool active = false;

   public:
    virtual void Activate() {
        this->active = true;
    }

    virtual void Deactivate() {
        this->active = false;
    }

    bool IsActive() const {
        return this->active;
    }

    // normally use page alpha to determine visibility
    // for divs, we can always return false
    virtual bool IsInvisible() const {
        return false;
    }

    virtual void Draw(Gdiplus::Graphics& graphics, const DrawParams& params) {
        if (this->IsInvisible()) {
            this->Deactivate();
            return;
        }

        if (app.page.fading) {
            this->Deactivate();
        } else {
            this->Activate();
        }

        this->DrawView(graphics, params);
    }

   protected:
    virtual void DrawView(Gdiplus::Graphics& graphics, const DrawParams& params) = 0;
};
