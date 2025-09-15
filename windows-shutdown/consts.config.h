#pragma once
#include <string_view>

namespace cfg {
// file
inline constexpr std::wstring_view FILE_NAME = L"config.txt";

// keys (original and lower-case forms)
inline constexpr std::string_view KEY_LANG = "language";
inline constexpr std::string_view KEY_ACTION = "action";
inline constexpr std::string_view KEY_DELAY = "delay";
inline constexpr std::string_view KEY_INSTRUCTION = "instruction";
inline constexpr std::string_view KEY_MENU_BUTTONS = "menuButtons";
inline constexpr std::string_view KEY_BACKGROUND_COLOR = "backgroundColor";
inline constexpr std::string_view KEY_COUNTDOWN_STYLE = "countdownStyle";

inline constexpr std::string_view LOWER_KEY_LANG = "language";
inline constexpr std::string_view LOWER_KEY_ACTION = "action";
inline constexpr std::string_view LOWER_KEY_DELAY = "delay";
inline constexpr std::string_view LOWER_KEY_INSTRUCTION = "instruction";
inline constexpr std::string_view LOWER_KEY_MENU_BUTTONS = "menubuttons";
inline constexpr std::string_view LOWER_KEY_BACKGROUND_COLOR = "backgroundcolor";
inline constexpr std::string_view LOWER_KEY_COUNTDOWN_STYLE = "countdownstyle";

// language values
inline constexpr std::string_view LANG_ZH = "zh";
inline constexpr std::string_view LANG_EN = "en";

// actions
inline constexpr std::string_view ACTION_SLEEP = "sleep";
inline constexpr std::string_view ACTION_SHUTDOWN = "shutdown";
inline constexpr std::string_view ACTION_RESTART = "restart";
inline constexpr std::string_view ACTION_LOCK = "lock";
inline constexpr std::string_view ACTION_NONE = "none";
inline constexpr std::string_view ACTION_SOME = "sleep, shutdown, restart, lock";

// menu buttons
inline constexpr std::string_view MENU_BUTTON_DONATE = "donate";
inline constexpr std::string_view MENU_BUTTON_CONFIG = "config";
inline constexpr std::string_view MENU_BUTTON_LOCK = "lock";
inline constexpr std::string_view MENU_BUTTON_SLEEP = "sleep";
inline constexpr std::string_view MENU_BUTTON_RESTART = "restart";
inline constexpr std::string_view MENU_BUTTON_SHUTDOWN = "shutdown";
inline constexpr std::string_view MENU_BUTTONS_SOME =
    "donate, config, lock, sleep, restart, shutdown";

// instruction
inline constexpr std::string_view INSTRUCTION_SHOW = "show";
inline constexpr std::string_view INSTRUCTION_HIDE = "hide";

// countdown styles
inline constexpr std::string_view COUNTDOWN_STYLE_NORMAL = "normal";
inline constexpr std::string_view COUNTDOWN_STYLE_STEINS_GATE = "nixietube";

// background color default
inline constexpr std::string_view BACKGROUND_COLOR_DEFAULT =
    "#00000034";  // ARGB same as macro BACKGROUND_COLOR below

// Wide (wstring_view) counterparts for code that needs wide strings
inline constexpr std::wstring_view KEY_LANG_W = L"language";
inline constexpr std::wstring_view KEY_ACTION_W = L"action";
inline constexpr std::wstring_view KEY_DELAY_W = L"delay";
inline constexpr std::wstring_view KEY_INSTRUCTION_W = L"instruction";
inline constexpr std::wstring_view KEY_MENU_BUTTONS_W = L"menuButtons";
inline constexpr std::wstring_view KEY_BACKGROUND_COLOR_W = L"backgroundColor";
inline constexpr std::wstring_view KEY_COUNTDOWN_STYLE_W = L"countdownStyle";

inline constexpr std::wstring_view LANG_ZH_W = L"zh";
inline constexpr std::wstring_view LANG_EN_W = L"en";

inline constexpr std::wstring_view ACTION_SLEEP_W = L"sleep";
inline constexpr std::wstring_view ACTION_SHUTDOWN_W = L"shutdown";
inline constexpr std::wstring_view ACTION_RESTART_W = L"restart";
inline constexpr std::wstring_view ACTION_LOCK_W = L"lock";
inline constexpr std::wstring_view ACTION_NONE_W = L"none";
inline constexpr std::wstring_view ACTION_SOME_W = L"sleep, shutdown, restart, lock";

inline constexpr std::wstring_view MENU_BUTTONS_SOME_W =
    L"donate, config, lock, sleep, restart, shutdown";

inline constexpr std::wstring_view INSTRUCTION_SHOW_W = L"show";
inline constexpr std::wstring_view INSTRUCTION_HIDE_W = L"hide";

inline constexpr std::wstring_view COUNTDOWN_STYLE_NORMAL_W = L"normal";
inline constexpr std::wstring_view COUNTDOWN_STYLE_STEINS_GATE_W = L"nixietube";

inline constexpr std::wstring_view BACKGROUND_COLOR_DEFAULT_W = L"#00000034";

enum class Lang : unsigned char { Zh, En };  // used in config and i18n
enum class Instruction : unsigned char { Show, Hide };
enum class CountdownStyle : unsigned char { Normal, SteinsGate };
inline constexpr int CFG_DEFAULT_DELAY = 5;

}  // namespace cfg
