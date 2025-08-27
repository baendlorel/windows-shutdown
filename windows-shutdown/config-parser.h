#pragma once
#include <string>

enum class Mode { IMMEDIATE, NORMAL };

struct WindowsShutdownConfig {
  Mode mode = Mode::IMMEDIATE;
};

WindowsShutdownConfig ParseConfigFile();
