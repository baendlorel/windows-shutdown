#pragma once

// font styles
constexpr int TEXT_SHADOW_RADIUS = 8;
constexpr int TEXT_SHADOW_RADIUS_STEP = 2;
constexpr int COUNT_DOWN_FONT_SIZE = 48;
constexpr int COUNT_DOWN_NUMBER_FONT_SIZE =
    64;  // font size for the large seconds number on the second line
constexpr int INSTRUCTION_FONT_SIZE = 20;
constexpr int FONT_OUTLINE_WIDTH = 3;
constexpr float SHADOW_OFFSET[8][2] = {
    {1.000f, 0.000f},    // 0
    {0.707f, 0.707f},    // 45
    {0.000f, 1.000f},    // 90
    {-0.707f, 0.707f},   // 135
    {-1.000f, 0.000f},   // 180
    {-0.707f, -0.707f},  // 225
    {0.000f, -1.000f},   // 270
    {0.707f, -0.707f}    // 315
};
