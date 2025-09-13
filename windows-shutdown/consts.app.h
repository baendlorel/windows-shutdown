#pragma once
enum class Action { None, Donate, Config, Lock, Sleep, Restart, Shutdown };

enum class Page { None, Home, Countdown, Donate };

enum class ElementTag { Div, A, Button };

constexpr int CLOSE_TIME = 500;  // ms
