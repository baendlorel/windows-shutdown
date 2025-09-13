#pragma once

// BUTTON
constexpr int BUTTON_RADIUS = 80;
constexpr int BUTTON_DIAMETER = BUTTON_RADIUS * 2;
constexpr int BUTTON_SHADOW_WIDTH = 22;  // This value is based on testing, not actual measurement
constexpr int BUTTON_TRUE_RADIUS_SQUARED =
(BUTTON_RADIUS - BUTTON_SHADOW_WIDTH) *
(BUTTON_RADIUS - BUTTON_SHADOW_WIDTH);  // squared value for hit test

constexpr int BUTTON_GAP = 5;
constexpr int BUTTON_MARGIN_LEFT = 0;
constexpr int BUTTON_MARGIN_TOP = -60;
constexpr int BUTTON_MARGIN_BOTTOM = 45;
constexpr int BUTTON_CENTER_DISTANCE = 2 * BUTTON_RADIUS + BUTTON_GAP;
