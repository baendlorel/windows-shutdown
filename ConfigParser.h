#pragma once
#include <string>

const std::string MODE_IMMEDIATELY = "immediately";
const std::string MODE_NORMAL = "normal";

struct WindowsShutdownConfig {
  std::string mode = MODE_NORMAL;
};

WindowsShutdownConfig ParseConfigFile();
