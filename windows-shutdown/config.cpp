#include "config.h"

#include <windows.h>

#include <algorithm>
#include <fstream>
#include <sstream>
#include <format>

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


void Config::Load() {
  std::wstring configPath = GetConfigPath();
  std::ifstream file(configPath);
  if (!file.is_open()) {
    // Create default config.ini
    std::string content = std::format(
        "# When loading is failed, we will use default config values.\n"
        "\n"
        "# `mode` options:\n"
        "# - `{}` (default): show menu to choose an action\n"
        "# - `{}` : shutdown instantly\n"
        "mode={}\n"
        "\n"
        "# Wait time (in seconds) before action\n"
        "delay={}\n",
        MODE_NORMAL, MODE_IMMEDIATE, MODE_NORMAL,
        DEFAULT_DELAY
    );
    std::ofstream out(configPath);
    out << content;
    out.close();
    return;
  }

  std::string line;
  while (std::getline(file, line)) {
    line = trim(line);
    if (line.empty() || line[0] == '#') {
      continue;
    }
    auto eq = line.find('=');
    if (eq == std::string::npos) {
      continue;
    }
    std::string key = trim(line.substr(0, eq));
    std::string value = trim(line.substr(eq + 1));
    if (key == "mode") {
      if (value == MODE_IMMEDIATE) {
        this->mode = Mode::IMMEDIATE;
      } else {
        this->mode = Mode::NORMAL;
      }
    }
    if (key == "delay") {
      try {
        this->delay = std::clamp(std::stoi(value), 0, 60);
      } catch (const std::invalid_argument&) {
        this->delay = DEFAULT_DELAY;
      }
    }
  }
}