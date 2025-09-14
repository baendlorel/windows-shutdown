#pragma once
enum class Action { None, Donate, Config, Lock, Sleep, Restart, Shutdown };

enum class Page { None, Home, Countdown, Donate };

enum class ElementTag { Div, A, Button };

constexpr int MAX_LOADSTRING = 100;

constexpr int COUNTDOWN_TIMER_ID = 1002;
