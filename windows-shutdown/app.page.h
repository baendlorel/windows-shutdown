#pragma once
#include "framework.h"
#include "style.fade.h"
#include "consts.app.h"

typedef unsigned char BYTE;

class AppPage {
    SINGLETON(AppPage)

   public:
    // Current page being displayed
    Page current = Page::None;

    // Page to fade into, normaly none
    Page next = Page::None;

    // Opacity of current page, 0-255
    BYTE alpha = 0;

    // Only consider one page fading to another, no need to distinguish fading out or in
    bool fading = false;

    BYTE GetPageAlpha(Page page) {
        if (!fading) {
            return (current == page) ? MAX_ALPHA : 0;
        }

        if (current == page) {
            return MAX_ALPHA - alpha;
        } else if (next == page) {
            return alpha;
        } else {
            return 0;
        }
    }

    // If hWnd is provided, start a fading timer
    void Start(Page next, HWND hWnd) {
        if (next == this->current) {
            return;
        }
        this->next = next;
        this->fading = true;
        this->alpha = 0;

        if (hWnd != nullptr) {
            SetTimer(hWnd, FADE_TIMER_ID, FRAME_TIME, NULL);
        }
    }

    void SetAlpha(BYTE alpha) {
        this->alpha = alpha;
        if (alpha == MAX_ALPHA) {
            this->current = this->next;
            this->next = Page::None;
            this->fading = false;
            // invoke and clear fade-end callback if present
            if (onFadeEnd) {
                onFadeEnd();
                onFadeEnd = {};
            }
        }
    }

    // Register a callback to be invoked once when a fade finishes (alpha reaches MAX_ALPHA).
    void OnFadeEnd(std::function<void()> cb) {
        onFadeEnd = std::move(cb);
    }

    bool isOpening() const {
        return this->current == Page::None && this->fading;
    }

    bool isClosing() const {
        return this->next == Page::None && this->fading;
    }

    BYTE GetBackgroundAlpha() const {
        if (isOpening()) {
            return this->alpha;
        }
        if (isClosing()) {
            return MAX_ALPHA - this->alpha;
        }
        return MAX_ALPHA;
    }

   private:
    std::function<void()> onFadeEnd;
};
