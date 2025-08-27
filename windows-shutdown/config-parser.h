#pragma once
#include <string>

enum class Mode { IMMEDIATELY, NORMAL };

struct WindowsShutdownConfig {
  Mode mode = Mode::IMMEDIATELY;
};

WindowsShutdownConfig ParseConfigFile();
