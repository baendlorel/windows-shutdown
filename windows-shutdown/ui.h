#pragma once
#include <memory>
#include "framework.h"
#include "consts/effects.h"

struct DrawTextParams {
    std::wstring text;
    Gdiplus::Font* font;
    Gdiplus::RectF* rect;
    bool manualAlign = true;
    Gdiplus::StringAlignment horizontalAlign;
    BYTE alpha = TARGET_ALPHA;  // overall alpha for this text
    Gdiplus::Color* color;
    Gdiplus::Color* shadowColor;
};

Gdiplus::Color ApplyAlpha(Gdiplus::Color* color, BYTE alpha);

std::unique_ptr<Gdiplus::ImageAttributes> ImageAttrWithAlpha(Gdiplus::Image* image, BYTE alpha);

void DrawUIText(Gdiplus::Graphics& graphics, DrawTextParams& params);

void DrawUITextShadow(Gdiplus::Graphics& graphics, DrawTextParams& params);

void DrawCachedUIText(Gdiplus::Graphics& graphics, DrawTextParams& params);