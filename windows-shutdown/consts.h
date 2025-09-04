#pragma once

// state
enum class Action { None, Sleep, Shutdown, Restart, Lock };

enum class FadeState { None, FadingIn, FadingOut };

// configs
#define CFG_KEY_LANG             "language"
#define CFG_KEY_ACTION           "action"
#define CFG_KEY_DELAY            "delay"
#define CFG_KEY_INSTRUCTION      "instruction"
#define CFG_KEY_BACKGROUND_COLOR "backgroundColor"

#define CFG_LANG_ZH "zh"
#define CFG_LANG_EN "en"

#define CFG_ACTION_SLEEP    "sleep"
#define CFG_ACTION_SHUTDOWN "shutdown"
#define CFG_ACTION_RESTART  "restart"
#define CFG_ACTION_LOCK     "lock"
#define CFG_ACTION_NONE     "none"
#define CFG_ACTION_SOME \
    CFG_ACTION_SLEEP ", " CFG_ACTION_SHUTDOWN ", " CFG_ACTION_RESTART ", " CFG_ACTION_LOCK

#define CFG_INSTRUCTION_SHOW   "show"
#define CFG_INSTRUCTION_HIDDEN "hidden"

enum class Lang { Zh, En };  // used in config and i18n
enum class Instruction { Show, Hidden };
constexpr int CFG_DEFAULT_DELAY = 4;

// config warnings
enum class ConfigWarning {
    None,
    InvalidLanguage,
    InvalidAction,
    InvalidInstruction,
    InvalidDelay,
    InvalidBackgroundColorFormat,
    InvalidBackgroundColorValue
};

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
    {1.000f, 0.000f},    // 0
    {0.707f, 0.707f},    // 45
    {0.000f, 1.000f},    // 90
    {-0.707f, 0.707f},   // 135
    {-1.000f, 0.000f},   // 180
    {-0.707f, -0.707f},  // 225
    {0.000f, -1.000f},   // 270
    {0.707f, -0.707f}    // 315
};

// ui/colors
#define BACKGROUND_COLOR       Gdiplus::Color(52, 0, 0, 0)
#define TEXT_COLOR             Gdiplus::Color(255, 255, 255, 255)
#define TEXT_WARN_COLOR        Gdiplus::Color(255, 255, 193, 7)
#define TEXT_SHADOW_COLOR      Gdiplus::Color(128, 0, 0, 0)
#define BUTTON_HIGHLIGHT_COLOR Gdiplus::Color(52, 0, 0, 0)