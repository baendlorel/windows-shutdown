#pragma once

// effects
constexpr int MAX_LOADSTRING = 100;   // ms
constexpr int FADEIN_DURATION = 150;  // ms
constexpr int FADEIN_INTERVAL = 10;   // ms
constexpr int FADEIN_TIMER_ID = 1001;
constexpr int FADEOUT_TIMER_ID = 1002;
constexpr int COUNTDOWN_TIMER_ID = 1003;  // New timer for countdown

// style
constexpr int BUTTON_RADIUS = 80;
constexpr int BUTTON_GAP = 85;
constexpr int BUTTON_MARGIN_BOTTOM = 20;
constexpr int COUNT_DOWN_FONT_SIZE = 48;
constexpr int INSTRUCTION_FONT_SIZE = 20;
constexpr int FONT_OUTLINE_WIDTH = 3;
constexpr float SHADOW_OFFSET[8][2] = {
    {1.000f, 0.000f},    // 0бу
    {0.707f, 0.707f},    // 45бу
    {0.000f, 1.000f},    // 90бу
    {-0.707f, 0.707f},   // 135бу
    {-1.000f, 0.000f},   // 180бу
    {-0.707f, -0.707f},  // 225бу
    {0.000f, -1.000f},   // 270бу
    {0.707f, -0.707f}   // 315бу
};