#pragma once
#include "framework.h"
#include "mini-ui.core.h"

Gdiplus::Color ApplyAlpha(Gdiplus::Color* color, BYTE alpha);

std::unique_ptr<Gdiplus::ImageAttributes> ImageAttrWithAlpha(Gdiplus::Image* image, BYTE alpha);

void DrawUIText(Gdiplus::Graphics& graphics, DrawTextParams& params);

void DrawUITextShadow(Gdiplus::Graphics& graphics, DrawTextParams& params);

void DrawCachedUIText(Gdiplus::Graphics& graphics, DrawTextParams& params);
