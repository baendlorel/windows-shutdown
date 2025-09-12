#pragma once
#include "framework.h"
#include "consts/color-set.h"
#include "app-state.h"
#include "i18n.h"

class View {
   protected:
    AppState& appState = AppState::GetInstance();
    I18N& i18n = I18N::GetInstance();
    ColorSet& colors = ColorSet::GetInstance();
    Page page = Page::None;
    bool active = false;

   public:
    View(Page page) : page(page) {};
    virtual ~View() = default;

   public:
    bool isInvisible() const {
        return appState.page.GetPageAlpha(this->page) == 0;
    }

    void Draw(Gdiplus::Graphics& graphics, int w, int h) {
        if (this->isInvisible()) {
            this->Deactivate();
            return;
        }

        if (appState.page.fading) {
            this->Deactivate();
        } else {
            this->Activate();
        }

        this->DrawView(graphics, w, h);
    }

    bool IsActive() const {
        return this->active;
    }

    virtual void Activate() {
        this->active = true;
    }

    virtual void Deactivate() {
        this->active = false;
    }

   private:
    virtual void DrawView(Gdiplus::Graphics& graphics, int w, int h) = 0;
};