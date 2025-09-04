#include "config.h"

#include <windows.h>

#include <algorithm>
#include <format>
#include <fstream>
#include <sstream>

std::string DefaultConfigZh() {
    // Create default config.ini
    std::string lang = std::format(
        "# 可填：{}, {}。 默认值和系统语言相同\n"
        "{}={}",
        CFG_LANG_ZH, CFG_LANG_EN, CFG_KEY_LANG, CFG_LANG_ZH);

    std::string mode = std::format(
        "# 可填： \n"
        "# - {}（默认）：显示菜单，可以点击要做的操作\n"
        "# - {}：延迟之后直接执行操作\n"
        "{}={}",
        CFG_ACTION_NONE, CFG_ACTION_SOME, CFG_KEY_ACTION, CFG_ACTION_NONE);

    std::string instruction = std::format(
        "# 可填：{}, {}（默认）\n"
        "{}={}",
        CFG_INSTRUCTION_HIDDEN, CFG_INSTRUCTION_SHOW, CFG_KEY_INSTRUCTION, CFG_INSTRUCTION_SHOW);

    std::string delay = std::format(
        "# 在执行操作之前等待这么多秒，默认{}秒\n"
        "{}={}",
        CFG_DEFAULT_DELAY, CFG_KEY_DELAY, CFG_DEFAULT_DELAY);

    return std::format(
        "# 加载配置失败时会使用默认配置。\n"
        "\n"
        "{}\n"
        "\n"
        "{}\n"
        "\n"
        "{}\n"
        "\n"
        "{}\n",
        lang, mode, instruction, delay);
}

std::string DefaultConfigEn() {
    // Create default config.ini
    std::string lang = std::format(
        "# Options: {}, {}. Default: same as your system\n"
        "{}={}",
        CFG_LANG_ZH, CFG_LANG_EN, CFG_KEY_LANG, CFG_LANG_ZH);

    std::string mode = std::format(
        "# Options: \n"
        "# - {} : (Default)show menu to choose an action\n"
        "# - {} : After delayed seconds, do it instantly\n"
        "{}={}",
        CFG_ACTION_NONE, CFG_ACTION_SOME, CFG_KEY_ACTION, CFG_ACTION_NONE);

    std::string instruction = std::format(
        "# Options: {}, {}(Default)\n"
        "{}={}",
        CFG_INSTRUCTION_HIDDEN, CFG_INSTRUCTION_SHOW, CFG_KEY_INSTRUCTION, CFG_INSTRUCTION_SHOW);

    std::string delay = std::format(
        "# Wait time (in seconds, default is {}s) before action.\n"
        "{}={}",
        CFG_DEFAULT_DELAY, CFG_KEY_DELAY, CFG_DEFAULT_DELAY);

    return std::format(
        "# When loading is failed, we will use default config values.\n"
        "\n"
        "{}\n"
        "\n"
        "{}\n"
        "\n"
        "{}\n"
        "\n"
        "{}\n",
        lang, mode, instruction, delay);
}

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

bool IsSysLangChinese() {
    LANGID langId = GetUserDefaultUILanguage();
    WORD primaryLang = PRIMARYLANGID(langId);
    return primaryLang == LANG_CHINESE;
}

Config::Config()
    : lang(IsSysLangChinese() ? Lang::Zh : Lang::En),
      action(Action::None),
      instruction(Instruction::Show),
      delay(CFG_DEFAULT_DELAY),
      bgColor(BACKGROUND_COLOR) {
    this->Load();
}

std::wstring Config::GetConfigPath() {
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

// Use this in case of returning to MSVC
std::string WStringToUtf8(const std::wstring& wstr) {
    if (wstr.empty()) return {};
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), nullptr, 0,
                                          nullptr, nullptr);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), &strTo[0], size_needed, nullptr,
                        nullptr);
    return strTo;
}

ConfigWarning Config::LoadKeyValue(std::string& key, std::string& value) {
    if (key == CFG_KEY_LANG) {
        if (value == CFG_LANG_EN) {
            this->lang = Lang::En;
            return ConfigWarning::None;
        }

        if (value == CFG_LANG_ZH) {
            this->lang = Lang::Zh;
            return ConfigWarning::None;
        }

        return ConfigWarning::InvalidLanguage;
    }

    if (key == CFG_KEY_ACTION) {
        if (value == CFG_ACTION_NONE) {
            this->action = Action::None;
            return ConfigWarning::None;
        }
        if (value == CFG_ACTION_SLEEP) {
            this->action = Action::Sleep;
            return ConfigWarning::None;
        }
        if (value == CFG_ACTION_SHUTDOWN) {
            this->action = Action::Shutdown;
            return ConfigWarning::None;
        }
        if (value == CFG_ACTION_RESTART) {
            this->action = Action::Restart;
            return ConfigWarning::None;
        }
        if (value == CFG_ACTION_LOCK) {
            this->action = Action::Lock;
            return ConfigWarning::None;
        }
        this->action = Action::None;
        return ConfigWarning::InvalidAction;
    }

    if (key == CFG_KEY_INSTRUCTION) {
        if (value == CFG_INSTRUCTION_SHOW || value == CFG_INSTRUCTION_HIDDEN) {
            this->instruction =
                value == CFG_INSTRUCTION_SHOW ? Instruction::Show : Instruction::Hidden;
            return static_cast<ConfigWarning>(-1);
        } else {
            return ConfigWarning::InvalidInstruction;
        }
    }

    if (key == CFG_KEY_DELAY) {
        try {
            this->delay = std::clamp(std::stoi(value), 0, 60);
            return ConfigWarning::None;
        } catch (const std::invalid_argument&) {
            this->delay = CFG_DEFAULT_DELAY;
            return ConfigWarning::InvalidDelay;
        }
    }

    if (key == CFG_KEY_BACKGROUND_COLOR) {
        if (value.size() == 9 && value[0] == '#') {
            try {
                BYTE r = std::stoi(value.substr(1, 2), nullptr, 16);
                BYTE g = std::stoi(value.substr(3, 2), nullptr, 16);
                BYTE b = std::stoi(value.substr(5, 2), nullptr, 16);
                BYTE a = std::stoi(value.substr(7, 2), nullptr, 16);
                this->bgColor = Gdiplus::Color(a, r, g, b);
                return ConfigWarning::None;
            } catch (...) {
                return ConfigWarning::InvalidBackgroundColorValue;
            }
        }

        if (value.size() == 7 && value[0] == '#') {
            try {
                BYTE r = std::stoi(value.substr(1, 2), nullptr, 16);
                BYTE g = std::stoi(value.substr(3, 2), nullptr, 16);
                BYTE b = std::stoi(value.substr(5, 2), nullptr, 16);
                this->bgColor = Gdiplus::Color(255, r, g, b);
                return ConfigWarning::None;
            } catch (...) {
                return ConfigWarning::InvalidBackgroundColorValue;
            }
        }

        return ConfigWarning::InvalidBackgroundColorFormat;
    }
}

void Config::Load() {
    std::wstring configPathW = this->GetConfigPath();
    std::string configPath = WStringToUtf8(configPathW);

    // Read config file as UTF-8
    std::ifstream file(configPath);
    if (!file.is_open()) {
        // If config file does not exist, create it with UTF-8 encoding
        std::string content = IsSysLangChinese() ? DefaultConfigZh() : DefaultConfigEn();
        std::ofstream out(configPath);
        // Write UTF-8 BOM for compatibility
        const unsigned char bom[] = {0xEF, 0xBB, 0xBF};
        out.write((const char*)bom, sizeof(bom));
        out << content;
        out.close();
        return;
    }

    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::string buffer(size, '\0');
    if (size > 0) file.read(&buffer[0], size);
    std::istringstream utf8stream(buffer);

    // Parse config file line by line (UTF-8)
    std::string line;
    while (std::getline(utf8stream, line)) {
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
        this->LoadKeyValue(key, value);
    }
}