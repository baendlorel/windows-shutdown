#pragma once
#include "consts/core.h"
#include "consts/effects.h"

typedef unsigned char BYTE;

struct AppPage {
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

    void Start(Page next) {
        if (next == this->current) {
            return;
        }
        this->next = next;
        this->fading = true;
        this->alpha = 0;
    }

    void SetAlpha(BYTE alpha) {
        this->alpha = alpha;
        if (alpha == MAX_ALPHA) {
            this->current = this->next;
            this->next = Page::None;
            this->fading = false;
        }
    }

    // common functions
    void GoHome() {
        Start(Page::Home);
    }
};
