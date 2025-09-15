#pragma once
#include "framework.h"
#include "style.fade.h"
#include "consts.app.h"

typedef unsigned char BYTE;

class AppPage {
    SINGLETON(AppPage)

   public:
    // Current page being displayed
    app::Page current = app::Page::None;

    // Page to fade into, normally none
    app::Page next = app::Page::None;

    // Opacity of current page, 0-255
    BYTE alpha = 0;

    // Only consider one page fading to another, no need to distinguish fading out or in
    bool fading = false;

    BYTE get_page_alpha(const app::Page page) const {
        if (!fading) {
            return (current == page) ? fade::MAX_ALPHA : 0;
        }

        if (current == page) {
            return fade::MAX_ALPHA - alpha;
        }

        if (next == page) {
            return alpha;
        }

        return 0;
    }

    // If hWnd is provided, start a fading timer
    void start(const app::Page next_page, const HWND hWnd) {
        if (next_page == this->current) {
            return;
        }
        this->next = next_page;
        this->fading = true;
        this->alpha = 0;

        if (hWnd != nullptr) {
            SetTimer(hWnd, fade::TIMER_ID, fade::FRAME_TIME, nullptr);
        }
    }

    void set_alpha(const BYTE a) {
        this->alpha = a;
        if (a == fade::MAX_ALPHA) {
            this->current = this->next;
            this->next = app::Page::None;
            this->fading = false;
            // invoke and clear fade-end callback if present
            if (on_fade_end_) {
                on_fade_end_();
                on_fade_end_ = {};
            }
        }
    }

    // Register a callback to be invoked once when a fade finishes (alpha reaches MAX_ALPHA).
    void on_fade_end(std::function<void()> cb) {
        on_fade_end_ = std::move(cb);
    }

    [[nodiscard]] bool is_opening() const {
        return this->current == app::Page::None && this->fading;
    }

    [[nodiscard]] bool is_closing() const {
        return this->next == app::Page::None && this->fading;
    }

    [[nodiscard]] BYTE get_background_alpha() const {
        if (is_opening()) {
            return this->alpha;
        }
        if (is_closing()) {
            return fade::MAX_ALPHA - this->alpha;
        }
        return fade::MAX_ALPHA;
    }

   private:
    std::function<void()> on_fade_end_;
};
