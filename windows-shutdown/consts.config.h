#pragma once
#include <string_view>

namespace CFG {
// file
constexpr std::wstring_view FILE_NAME = L"config.txt";

// keys (original and lower-case forms)
constexpr std::string_view KEY_LANG = "language";
constexpr std::string_view KEY_ACTION = "action";
constexpr std::string_view KEY_DELAY = "delay";
constexpr std::string_view KEY_INSTRUCTION = "instruction";
constexpr std::string_view KEY_MENU_BUTTONS = "menuButtons";
constexpr std::string_view KEY_BACKGROUND_COLOR = "backgroundColor";
constexpr std::string_view KEY_COUNTDOWN_STYLE = "countdownStyle";

constexpr std::string_view LOWER_KEY_LANG = "language";
constexpr std::string_view LOWER_KEY_ACTION = "action";
constexpr std::string_view LOWER_KEY_DELAY = "delay";
constexpr std::string_view LOWER_KEY_INSTRUCTION = "instruction";
constexpr std::string_view LOWER_KEY_MENU_BUTTONS = "menubuttons";
constexpr std::string_view LOWER_KEY_BACKGROUND_COLOR = "backgroundcolor";
constexpr std::string_view LOWER_KEY_COUNTDOWN_STYLE = "countdownstyle";

// language values
constexpr std::string_view LANG_ZH = "zh";
constexpr std::string_view LANG_EN = "en";

// actions
constexpr std::string_view ACTION_SLEEP = "sleep";
constexpr std::string_view ACTION_SHUTDOWN = "shutdown";
constexpr std::string_view ACTION_RESTART = "restart";
constexpr std::string_view ACTION_LOCK = "lock";
constexpr std::string_view ACTION_NONE = "none";
constexpr std::string_view ACTION_SOME = "sleep, shutdown, restart, lock";

// menu buttons
constexpr std::string_view MENU_BUTTON_DONATE = "donate";
constexpr std::string_view MENU_BUTTON_CONFIG = "config";
constexpr std::string_view MENU_BUTTON_LOCK = "lock";
constexpr std::string_view MENU_BUTTON_SLEEP = "sleep";
constexpr std::string_view MENU_BUTTON_RESTART = "restart";
constexpr std::string_view MENU_BUTTON_SHUTDOWN = "shutdown";
constexpr std::string_view MENU_BUTTONS_SOME = "donate, config, lock, sleep, restart, shutdown";

// instruction
constexpr std::string_view INSTRUCTION_SHOW = "show";
constexpr std::string_view INSTRUCTION_HIDE = "hide";

// countdown styles
constexpr std::string_view COUNTDOWN_STYLE_NORMAL = "normal";
constexpr std::string_view COUNTDOWN_STYLE_STEINS_GATE = "nixietube";

// background color default
constexpr std::string_view BACKGROUND_COLOR_DEFAULT =
    "#00000034";  // ARGB same as macro BACKGROUND_COLOR below

// Wide (wstring_view) counterparts for code that needs wide strings
constexpr std::wstring_view KEY_LANG_W = L"language";
constexpr std::wstring_view KEY_ACTION_W = L"action";
constexpr std::wstring_view KEY_DELAY_W = L"delay";
constexpr std::wstring_view KEY_INSTRUCTION_W = L"instruction";
constexpr std::wstring_view KEY_MENU_BUTTONS_W = L"menuButtons";
constexpr std::wstring_view KEY_BACKGROUND_COLOR_W = L"backgroundColor";
constexpr std::wstring_view KEY_COUNTDOWN_STYLE_W = L"countdownStyle";

constexpr std::wstring_view LANG_ZH_W = L"zh";
constexpr std::wstring_view LANG_EN_W = L"en";

constexpr std::wstring_view ACTION_SLEEP_W = L"sleep";
constexpr std::wstring_view ACTION_SHUTDOWN_W = L"shutdown";
constexpr std::wstring_view ACTION_RESTART_W = L"restart";
constexpr std::wstring_view ACTION_LOCK_W = L"lock";
constexpr std::wstring_view ACTION_NONE_W = L"none";
constexpr std::wstring_view ACTION_SOME_W = L"sleep, shutdown, restart, lock";

constexpr std::wstring_view MENU_BUTTONS_SOME_W = L"donate, config, lock, sleep, restart, shutdown";

constexpr std::wstring_view INSTRUCTION_SHOW_W = L"show";
constexpr std::wstring_view INSTRUCTION_HIDE_W = L"hide";

constexpr std::wstring_view COUNTDOWN_STYLE_NORMAL_W = L"normal";
constexpr std::wstring_view COUNTDOWN_STYLE_STEINS_GATE_W = L"nixietube";

constexpr std::wstring_view BACKGROUND_COLOR_DEFAULT_W = L"#00000034";

enum class Lang { Zh, En };  // used in config and i18n
enum class Instruction { Show, Hide };
enum class CountdownStyle { Normal, SteinsGate };
constexpr int CFG_DEFAULT_DELAY = 5;

// config warnings
enum class Warning {
    None,
    UnknownConfigKey,  // key not recognized
    NotConfigEntry,    // neither comment nor key-value pair
    InvalidLanguage,
    InvalidAction,
    InvalidInstruction,
    InvalidMenuButton,
    InvalidCountdownStyle,
    InvalidDelay,
    InvalidBackgroundColorFormat,
    InvalidBackgroundColorValue
};

struct WarnInfo {
    Warning warning;
    short lineNo;
};

constexpr int WARNING_X = 40;
constexpr int WARNING_Y = 40;
constexpr int WARNING_FONT_SIZE = 16;
}  // namespace CFG
