#pragma once
#include "framework.h"
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