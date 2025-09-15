#pragma once

// menu buttons
namespace menu_button_style {

// radius of the button
inline constexpr int RADIUS = 80;

// diameter of the button
inline constexpr int DIAMETER = RADIUS * 2;

// This value is based on testing, not actual measurement
inline constexpr int SHADOW_WIDTH = 22;

// squared value for hit test
inline constexpr int TRUE_R_SQUARED = (RADIUS - SHADOW_WIDTH) * (RADIUS - SHADOW_WIDTH);

// gap between 2 button's edges
inline constexpr int GAP = 5;
inline constexpr int MARGIN_LEFT = 0;
inline constexpr int MARGIN_TOP = -60;
inline constexpr int MARGIN_BOTTOM = 45;
inline constexpr int CENTER_DIST = 2 * RADIUS + GAP;

}  // namespace menu_button_style
