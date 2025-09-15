#pragma once

namespace warning {

// config warnings
enum class Code : unsigned char {
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

struct Info {
    Code warning_code;
    short line_no;

    Info(const Code warning_code, const short line_no) {
        this->warning_code = warning_code;
        this->line_no = line_no;
    }
};

inline constexpr int DISPLAY_X = 40;
inline constexpr int DISPLAY_Y = 40;
inline constexpr int FONT_SIZE = 16;
}  // namespace warning