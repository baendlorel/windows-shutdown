#pragma once

// configs
#define CFG_FILE_NAME L"config.txt"

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

#define CFG_INSTRUCTION_SHOW "show"
#define CFG_INSTRUCTION_HIDE "hide"

#define CFG_BACKGROUND_COLOR_DEFAULT "#00000034"  // ARGB same as macro BACKGROUND_COLOR below

enum class Lang { Zh, En };  // used in config and i18n
enum class Instruction { Show, Hide };
constexpr int CFG_DEFAULT_DELAY = 3;

// config warnings
enum class ConfigWarning {
    None,
    UnknownConfigKey,  // key not recognized
    NotConfigEntry,    // neither comment nor key-value pair
    InvalidLanguage,
    InvalidAction,
    InvalidInstruction,
    InvalidDelay,
    InvalidBackgroundColorFormat,
    InvalidBackgroundColorValue
};

struct ConfigWarnInfo {
    ConfigWarning warning;
    short lineNo;
};

constexpr int CFG_WARNING_X = 40;
constexpr int CFG_WARNING_Y = 40;
constexpr int CFG_WARNING_FONT_SIZE = 16;
