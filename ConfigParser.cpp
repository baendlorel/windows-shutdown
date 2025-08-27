#include "ConfigParser.h"

#include <algorithm>
#include <fstream>
#include <sstream>

static std::string trim(const std::string& s) {
  auto start = s.begin();
  while (start != s.end() && isspace(*start)) ++start;
  auto end = s.end();
  do {
    --end;
  } while (end >= start && isspace(*end));
  return (start <= end) ? std::string(start, end + 1) : "";
}

WindowsShutdownConfig ParseConfigFile(const std::wstring& path) {
  WindowsShutdownConfig config;
  std::ifstream file(path);
  std::string line;
  while (std::getline(file, line)) {
    // Remove leading/trailing spaces
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
      config.mode = value;
    }
  }
  return config;
}
