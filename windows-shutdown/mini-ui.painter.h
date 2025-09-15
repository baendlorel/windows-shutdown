#pragma once
#include "framework.h"
#include "mini-ui.core.h"

Gdiplus::Color apply_alpha(const Gdiplus::Color* color, BYTE alpha);

std::unique_ptr<Gdiplus::ImageAttributes> image_attr_with_alpha(const Gdiplus::Image* image,
                                                                BYTE alpha);

void draw_ui_text(Gdiplus::Graphics& graphics, const DrawTextParams& params);

void draw_ui_text_shadow(Gdiplus::Graphics& graphics, const DrawTextParams& params);

void draw_cached_ui_text(Gdiplus::Graphics& graphics, const DrawTextParams& params);
