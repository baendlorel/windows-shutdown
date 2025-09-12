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

   public:
    virtual ~View() = default;

   public:
    bool isInvisible() const {
        return appState.page.GetPageAlpha(this->page) == 0;
    }

    void Draw(Gdiplus::Graphics& graphics, int w, int h) {
        if (this->isInvisible()) {
            return;
        }
        this->DrawView(graphics, w, h);
    }

   private:
    virtual void DrawView(Gdiplus::Graphics& graphics, int w, int h) = 0;
};