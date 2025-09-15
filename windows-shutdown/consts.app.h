#pragma once
namespace app {

enum class Action : unsigned char { None, Donate, Config, Lock, Sleep, Restart, Shutdown };

enum class Page : unsigned char { None, Home, Countdown, Donate };

enum class ElementTag : unsigned char { Div, A, Button };

inline constexpr int MAX_LOAD_STRING = 100;

inline constexpr int COUNTDOWN_TIMER_ID = 1002;

}  // namespace app
