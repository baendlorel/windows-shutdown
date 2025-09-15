#pragma once
#include "framework.h"
#include "style.fade.h"
#include "app.core.h"

struct DrawTextParams {
    std::wstring text;
    const Gdiplus::Font* font;
    const Gdiplus::RectF* rect;
    bool manual_align = true;  // controls whether to use horizontalAlign or always near
    Gdiplus::StringAlignment horizontal_align;
    BYTE alpha = FADE::MAX_ALPHA;  // overall alpha for this text
    const Gdiplus::Color* color;
    const Gdiplus::Color* shadow_color;
};

struct DrawParams {
    unsigned char alpha;

    // optional, for text drawing or image drawing
    Gdiplus::RectF* rect = nullptr;

    // optional, for text drawing
    DrawTextParams* text_params = nullptr;
};