#pragma once
#include <string>

const std::string MODE_IMMEDIATELY = "immediately";
const std::string MODE_NORMAL = "normal";

struct WindowsShutdownConfig {
  std::string mode = MODE_NORMAL;
};

std::string trim(const std::string& s);
std::wstring GetConfigPath();
WindowsShutdownConfig ParseConfigFile();
