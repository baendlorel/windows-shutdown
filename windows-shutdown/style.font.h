#pragma once

// font styles
namespace font_style {

inline constexpr int SHADOW_ALPHA = 160;  // max alpha for text shadow
inline constexpr int SHADOW_RADIUS = 8;
inline constexpr int SHADOW_RADIUS_STEP = 2;

inline constexpr int COUNT_DOWN_FONT_SIZE = 48;
// font size for the large seconds number on the second line
inline constexpr int COUNT_DOWN_NUMBER_FONT_SIZE = 64;

inline constexpr int INSTRUCTION_FONT_SIZE = 20;
inline constexpr int FONT_OUTLINE_WIDTH = 3;

inline constexpr float SHADOW_OFFSET[16][2] = {
    {1.000f, 0.000f},    // 0
    {0.924f, 0.383f},    // 22.5
    {0.707f, 0.707f},    // 45
    {0.383f, 0.924f},    // 67.5
    {0.000f, 1.000f},    // 90
    {-0.383f, 0.924f},   // 112.5
    {-0.707f, 0.707f},   // 135
    {-0.924f, 0.383f},   // 157.5
    {-1.000f, 0.000f},   // 180
    {-0.924f, -0.383f},  // 202.5
    {-0.707f, -0.707f},  // 225
    {-0.383f, -0.924f},  // 247.5
    {0.000f, -1.000f},   // 270
    {0.383f, -0.924f},   // 292.5
    {0.707f, -0.707f},   // 315
    {0.924f, -0.383f}    // 337.5
};

}  // namespace font_style
