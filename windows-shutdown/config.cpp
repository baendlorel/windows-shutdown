#include "config.h"

#include <windows.h>

#include <algorithm>
#include <format>
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

bool IsSystemLanguageChinese() {
  LANGID langId = GetUserDefaultUILanguage();
  WORD primaryLang = PRIMARYLANGID(langId);
  return primaryLang == LANG_CHINESE;
}

Config::Config() {
  this->lang = IsSystemLanguageChinese() ? Lang::ZH : Lang::EN;
  this->mode = Mode::NORMAL;
  this->delay = CFG_DEFAULT_DELAY;
  this->Load();
}

void Config::Load() {
  std::wstring configPath = GetConfigPath();
  std::ifstream file(configPath);
  if (!file.is_open()) {
    // Create default config.ini
    std::string lang = std::format(
        "# '{}' options: 'zh', 'en'. Default: same as your system\n"
        "{}={}",
        CFG_KEY_LANG, CFG_KEY_LANG, CFG_LANG_ZH);

    std::string mode = std::format(
        "# '{}' options: (Default: {})\n"
        "# - '{}' : show menu to choose an action\n"
        "# - '{}' : shutdown instantly\n"
        "{}={}",
        CFG_KEY_MODE, CFG_MODE_NORMAL, CFG_MODE_NORMAL, CFG_MODE_IMMEDIATE,
        CFG_KEY_MODE, CFG_MODE_NORMAL);

    std::string delay = std::format(
        "# Wait time (in seconds) before action. Default: {}\n"
        "{}={}",
        CFG_DEFAULT_DELAY, CFG_KEY_DELAY, CFG_DEFAULT_DELAY);

    std::string content = std::format(
        "# When loading is failed, we will use default config values.\n"
        "\n"
        "{}\n"
        "\n"
        "{}\n"
        "\n"
        "{}\n",
        lang, mode, delay);
    std::ofstream out(configPath);
    out << content;
    out.close();
    return;
  }

  std::string line;

  enum ConfigKey {
    LANG,
    MODE,
    DELAY,
  }; 

  bool isSet[3] = {false, false, false};

  while (std::getline(file, line)) {
    if (line.empty()) {
      continue;
    }
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

    if (key == CFG_KEY_LANG) {
      this->lang = value == CFG_LANG_EN ? Lang::EN : Lang::ZH;
      isSet[LANG] = true;    
    } else if (key == CFG_KEY_MODE) {
      this->mode = value == CFG_MODE_IMMEDIATE ? Mode::IMMEDIATE : Mode::NORMAL;
      isSet[MODE] = true;
    } else if (key == CFG_KEY_DELAY) {
      try {
        this->delay = std::clamp(std::stoi(value), 0, 60);
      } catch (const std::invalid_argument&) {
        this->delay = CFG_DEFAULT_DELAY;
      }
      isSet[DELAY] = true;
    }
  }

  if (!isSet[LANG]) {
    this->lang = IsSystemLanguageChinese() ? Lang::ZH : Lang::EN;
  }
  if (!isSet[MODE]) {
    this->mode = Mode::NORMAL;
  }
  if (!isSet[DELAY]) {
    this->delay = CFG_DEFAULT_DELAY;
  }
}