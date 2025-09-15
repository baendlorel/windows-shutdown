#pragma once
namespace app {

enum class Action : std::uint8_t { None, Donate, Config, Lock, Sleep, Restart, Shutdown };

enum class Page : std::uint8_t { None, Home, Countdown, Donate };

enum class ElementTag : std::uint8_t { Div, A, Button };

inline constexpr int MAX_LOAD_STRING = 100;

inline constexpr int COUNTDOWN_TIMER_ID = 1002;

}  // namespace app
