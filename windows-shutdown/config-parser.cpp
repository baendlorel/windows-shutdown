#include "config-parser.h"

#include <windows.h>

#include <algorithm>
#include <fstream>
#include <sstream>

std::string trim(const std::string& s) {
  auto start = s.begin();
  while (start != s.end() && isspace(*start)) {
    ++start;
  }
  auto end = s.end();
  do {
    --end;
  } while (end >= start && isspace(*end));

  return (start <= end) ? std::string(start, end + 1) : "";
}

std::wstring GetConfigPath() {
  wchar_t exePath[MAX_PATH] = {0};
  GetModuleFileNameW(NULL, exePath, MAX_PATH);
  std::wstring path(exePath);
  auto pos = path.find_last_of(L"\\/");
  if (pos != std::wstring::npos) {
    path = path.substr(0, pos + 1);
  } else {
    path = L"";
  }
  path += L"config.ini";
  return path;
}

const std::string MODE_IMMEDIATE = "immediate";
const std::string MODE_NORMAL = "normal";

WindowsShutdownConfig ParseConfigFile() {
  WindowsShutdownConfig config;
  config.mode = Mode::NORMAL;  // default value
  std::wstring configPath = GetConfigPath();
  std::ifstream file(configPath);
  if (!file.is_open()) {
    // Create default config.ini
    std::ofstream out(configPath);
    out << "# If an invalid config file or invalid values are detected, we will use default values."  << std::endl;
    out << "# `mode` can be \"" << MODE_IMMEDIATE << "\" and \"" << MODE_NORMAL
        << "\""
        << std::endl;
    out << "# \"" << MODE_NORMAL
        << "\"(default): will popup control buttons to choose whether shutdown "
           "or restart"
        << std::endl;
    out << "# \"" << MODE_IMMEDIATE << "\": will shutdown immediately"
        << std::endl;
    out << "mode=" << MODE_NORMAL << std::endl;
    out.close();
    return config;
  }

  std::string line;
  while (std::getline(file, line)) {
    line = trim(line);
    if (line.empty() || line[0] == '#') continue;
    auto eq = line.find('=');
    if (eq == std::string::npos) continue;
    std::string key = trim(line.substr(0, eq));
    std::string value = trim(line.substr(eq + 1));
    if (key == "mode") {
      if (value == MODE_IMMEDIATE) {
        config.mode = Mode::IMMEDIATE;
      } else {
        config.mode = Mode::NORMAL;
      }
    }
  }
  return config;
}
