#pragma once
#include "framework.h"
#include "style.color.h"
#include "style.fade.h"
#include "app.core.h"

struct DrawTextParams {
    std::wstring text;
    Gdiplus::Font* font;
    Gdiplus::RectF* rect;
    bool manualAlign = true;  // controls whether to use horizontalAlign or always near
    Gdiplus::StringAlignment horizontalAlign;
    BYTE alpha = FADE::MAX_ALPHA;  // overall alpha for this text
    Gdiplus::Color* color;
    Gdiplus::Color* shadowColor;
};

struct DrawParams {
    unsigned char alpha;

    // optional, for text drawing or image drawing
    Gdiplus::RectF* rect = nullptr;

    // optional, for text drawing
    DrawTextParams* textParams = nullptr;
};

class Element {
   public:
    Element() = default;

   protected:
    App& app = App::GetInstance();
    ColorSet& colors = ColorSet::GetInstance();

    // when alpha is MAX_ALPHA, active state is true
    bool active = false;

   public:
    void Activate() {
        this->active = true;
    }

    void Deactivate() {
        this->active = false;
    }

    bool IsActive() const {
        return this->active;
    }

    // normally use page alpha to determine visibility
    virtual bool IsInvisible() const {
        return false;
    }

    virtual void Draw(Gdiplus::Graphics& graphics, DrawParams& params) {
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
    virtual void DrawView(Gdiplus::Graphics& graphics, DrawParams& params) = 0;
};

class Div : public Element {
   protected:
    App& app = App::GetInstance();
    ColorSet& colors = ColorSet::GetInstance();

   public:
    Div(ElementTag tag, const Gdiplus::RectF& rect) : tag(tag), rect(rect) {};
    virtual ~Div() = default;

   private:
    std::function<void(HWND)> onClickCallback;

   public:
    ElementTag tag;
    Gdiplus::RectF rect;  // Position and size
    bool hovered = false;

    virtual bool MouseHit(int mx, int my) {
        this->hovered =
            mx >= rect.X && mx <= rect.Y && my >= rect.GetTop() && my <= rect.GetBottom();
        return this->hovered;
    }

    void OnClick(std::function<void(HWND)> cb) {
        this->onClickCallback = std::move(cb);
    }

    void TriggerClick(HWND hwnd) {
        if (this->onClickCallback && this->active) {
            this->onClickCallback(hwnd);
        }
    }
};

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

Gdiplus::Color ApplyAlpha(Gdiplus::Color* color, BYTE alpha);

std::unique_ptr<Gdiplus::ImageAttributes> ImageAttrWithAlpha(Gdiplus::Image* image, BYTE alpha);

void DrawUIText(Gdiplus::Graphics& graphics, DrawTextParams& params);

void DrawUITextShadow(Gdiplus::Graphics& graphics, DrawTextParams& params);

void DrawCachedUIText(Gdiplus::Graphics& graphics, DrawTextParams& params);
