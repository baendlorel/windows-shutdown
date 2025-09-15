#pragma once
#include "framework.h"
#include "style.font.h"

// colors are like those in Bootstrap 5.3
namespace color_set {

// used for background color and button highlight, or other things else in the future
inline constexpr int DEFAULT_ALPHA = 52;
inline const auto BACKGROUND = Gdiplus::Color(DEFAULT_ALPHA, 0, 0, 0);
inline const auto TEXT_LIGHT = Gdiplus::Color(255, 248, 249, 255);
inline const auto TEXT_WARN = Gdiplus::Color(255, 255, 193, 7);
inline const auto TEXT_DANGER = Gdiplus::Color(255, 255, 151, 163);
inline const auto TEXT_SHADOW = Gdiplus::Color(font_style::SHADOW_ALPHA, 0, 0, 0);
inline const auto BUTTON_HIGHLIGHT = Gdiplus::Color(DEFAULT_ALPHA, 255, 255, 255);

}  // namespace color_set
