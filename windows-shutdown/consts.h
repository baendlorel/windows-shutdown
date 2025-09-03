#pragma once

// state
enum class Action { None, Sleep, Shutdown, Restart, Lock };

enum class FadeState { None, FadingIn, FadingOut };

// configs
#define CFG_KEY_LANG "lang"
#define CFG_KEY_MODE "mode"
#define CFG_KEY_DELAY "delay"

#define CFG_LANG_ZH "zh"
#define CFG_LANG_EN "en"

#define CFG_MODE_IMMEDIATE "immediate"
#define CFG_MODE_NORMAL "normal"

enum class Mode { Immediate, Normal };
enum class Lang { Zh, En };  // used in config and i18n
constexpr int CFG_DEFAULT_DELAY = 3;

// effects
constexpr int MAX_LOADSTRING = 100;   // ms
constexpr int FADEIN_DURATION = 120;  // ms
constexpr int FADEIN_INTERVAL = 10;   // ms
constexpr int FADEIN_TIMER_ID = 1001;
constexpr int FADEOUT_TIMER_ID = 1002;
constexpr int COUNTDOWN_TIMER_ID = 1003;  // New timer for countdown

// ui/style
enum class Button {
    Config,
    Lock,
    Sleep,
    Restart,
    Shutdown,
};
constexpr int BUTTON_COUNT = 5;
constexpr int BUTTON_RADIUS = 80;
constexpr int BUTTON_SHADOW_WIDTH = 22;  // This value is based on testing, not actual measurement
constexpr int BUTTON_GAP = 5;
constexpr int BUTTON_CENTER_DISTANCE = 2 * BUTTON_RADIUS + BUTTON_GAP;

constexpr int BUTTON_MARGIN_LEFT = 0;
constexpr int BUTTON_MARGIN_TOP = -60;
constexpr int BUTTON_MARGIN_BOTTOM = 45;
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
    {0.707f, -0.707f}    // 315бу
};
