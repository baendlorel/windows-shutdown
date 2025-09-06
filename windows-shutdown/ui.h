#pragma once
#include <unordered_map>
#include "framework.h"

struct DrawTextParams {
    std::wstring text;
    Gdiplus::Font* font;
    Gdiplus::RectF* rect;
    bool manualAlign = true;
    Gdiplus::StringAlignment horizontalAlign;
    Gdiplus::Color* color;
    Gdiplus::Color* shadowColor;
};

void DrawUIText(Gdiplus::Graphics& graphics, DrawTextParams& params);

void DrawUITextShadow(Gdiplus::Graphics& graphics, DrawTextParams& params);

Gdiplus::Bitmap* UITextToBitmap(Gdiplus::Graphics& graphics, DrawTextParams& params);

void DrawCachedUIText(Gdiplus::Graphics& graphics, DrawTextParams& params);