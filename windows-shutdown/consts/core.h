#pragma once
enum class Action { None, Donate, Config, Lock, Sleep, Restart, Shutdown };

enum class Page { None, Main, Countdown, Donate };

constexpr int CLOSE_TIME = 500;  // ms