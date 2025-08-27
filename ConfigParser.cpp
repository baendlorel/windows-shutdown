#include "ConfigParser.h"

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

WindowsShutdownConfig ParseConfigFile() {
  WindowsShutdownConfig config;
  config.mode = MODE_IMMEDIATELY;  // default value
  std::wstring configPath = GetConfigPath();
  std::ifstream file(configPath);
  if (!file.is_open()) {
    // Create default config.ini
    std::ofstream out(configPath);
    out << "mode=" << MODE_IMMEDIATELY << std::endl;
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
      if (value == MODE_IMMEDIATELY) {
        config.mode = MODE_IMMEDIATELY;
      } else if (value == MODE_NORMAL) {
        config.mode = MODE_NORMAL;
      } else {
        config.mode = MODE_IMMEDIATELY;
      }
    }
  }
  return config;
}
