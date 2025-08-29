#pragma once
#include <string>

enum class Mode { IMMEDIATE, NORMAL };
constexpr int DEFAULT_DELAY = 5;

struct WindowsShutdownConfig {
  Mode mode = Mode::IMMEDIATE;
  int delay = DEFAULT_DELAY;
};

WindowsShutdownConfig ParseConfigFile();
