#pragma once
#include <string>

enum class Mode { IMMEDIATE, NORMAL };
constexpr int DEFAULT_DELAY = 5;

struct WindowsShutdownConfig {
  Mode mode = Mode::NORMAL;
  int delay = DEFAULT_DELAY;
};

void ParseConfigFile(WindowsShutdownConfig& config);
