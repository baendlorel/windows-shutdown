#pragma once

// menu buttons
namespace MENU_BUTTON {

// radius of the button
inline constexpr int R = 80;

// diameter of the button
inline constexpr int D = R * 2;

// This value is based on testing, not actual measurement
inline constexpr int SHADOW_WIDTH = 22;

// squared value for hit test
inline constexpr int TRUE_R_SQUARED = (R - SHADOW_WIDTH) * (R - SHADOW_WIDTH);

// gap between 2 button's edges
inline constexpr int GAP = 5;
inline constexpr int MARGIN_LEFT = 0;
inline constexpr int MARGIN_TOP = -60;
inline constexpr int MARGIN_BOTTOM = 45;
inline constexpr int CENTER_DIST = 2 * R + GAP;

}  // namespace MENU_BUTTON
