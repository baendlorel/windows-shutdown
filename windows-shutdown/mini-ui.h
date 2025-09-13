#pragma once
#include "framework.h"
#include "style.color.h"
#include "style.fade.h"
#include "app.state.h"
#include "i18n.h"

struct DrawTextParams {
    std::wstring text;
    Gdiplus::Font* font;
    Gdiplus::RectF* rect;
    bool manualAlign = true;
    Gdiplus::StringAlignment horizontalAlign;
    BYTE alpha = MAX_ALPHA;  // overall alpha for this text
    Gdiplus::Color* color;
    Gdiplus::Color* shadowColor;
};

struct DrawParams {
    unsigned char alpha;

    // optional, for text drawing or image drawing
    Gdiplus::RectF* rect = nullptr;
    DrawTextParams* textParams = nullptr;
};

class Div {
   protected:
    AppState& appState = AppState::GetInstance();
    AppPage& appPage = AppPage::GetInstance();
    I18N& i18n = I18N::GetInstance();
    ColorSet& colors = ColorSet::GetInstance();

    // when alpha is MAX_ALPHA, active state is true
    bool active = false;

   public:
    Div(ElementTag tag, const Gdiplus::RectF& rect) : tag(tag), rect(rect) {};
    virtual ~Div() = default;

   private:
    std::function<void(HWND)> onClickCallback;

   public:
    ElementTag tag;
    Gdiplus::RectF rect;  // Position and size

    virtual bool MouseHit(int mx, int my) const {
        return mx >= rect.X && mx <= rect.Y && my >= rect.GetTop() && my <= rect.GetBottom();
    }

    virtual void Draw(Gdiplus::Graphics& graphics, DrawParams& params) = 0;

    void OnClick(std::function<void(HWND)> cb) {
        this->onClickCallback = std::move(cb);
    }

    void TriggerClick(HWND hwnd) {
        if (this->onClickCallback && this->active) {
            this->onClickCallback(hwnd);
        }
    }

    void Activate() {
        this->active = true;
    }

    void Deactivate() {
        this->active = false;
    }

    bool IsActive() const {
        return this->active;
    }
};

class View {
   protected:
    AppConfig& appState = AppConfig::GetInstance();
    AppState& appState = AppState::GetInstance();
    AppPage& appPage = AppPage::GetInstance();
    I18N& i18n = I18N::GetInstance();
    ColorSet& colors = ColorSet::GetInstance();

    Page page = Page::None;
    bool active = false;

   public:
    View(Page page) : page(page) {};
    virtual ~View() = default;

   public:
    bool isInvisible() const {
        return appPage.GetPageAlpha(this->page) == 0;
    }

    void Draw(Gdiplus::Graphics& graphics, int w, int h) {
        if (this->isInvisible()) {
            this->Deactivate();
            return;
        }

        if (appPage.fading) {
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

Gdiplus::Color ApplyAlpha(Gdiplus::Color* color, BYTE alpha);

std::unique_ptr<Gdiplus::ImageAttributes> ImageAttrWithAlpha(Gdiplus::Image* image, BYTE alpha);

void DrawUIText(Gdiplus::Graphics& graphics, DrawTextParams& params);

void DrawUITextShadow(Gdiplus::Graphics& graphics, DrawTextParams& params);

void DrawCachedUIText(Gdiplus::Graphics& graphics, DrawTextParams& params);
