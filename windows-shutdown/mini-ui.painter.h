#pragma once
#include "framework.h"
#include "mini-ui.core.h"

namespace painter {

Gdiplus::Color apply_alpha(const Gdiplus::Color* color, BYTE alpha);

std::unique_ptr<Gdiplus::ImageAttributes> image_attr_with_alpha(const Gdiplus::Image* image,
                                                                const BYTE alpha);

Gdiplus::Bitmap* text_to_bitmap(const Gdiplus::Graphics& graphics, const DrawTextParams& params);

void draw_text(Gdiplus::Graphics& graphics, const DrawTextParams& params);

void draw_text_shadow(Gdiplus::Graphics& graphics, const DrawTextParams& params);

void draw_cached_text(Gdiplus::Graphics& graphics, const DrawTextParams& params);

}  // namespace painter
