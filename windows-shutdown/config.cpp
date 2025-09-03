#include "config.h"

#include <windows.h>

#include <algorithm>
#include <format>
#include <fstream>
#include <sstream>

std::string DefaultConfigZh() {
    // Create default config.ini
    std::string lang = std::format(
        "# 可选项：{}, {}。 默认值和系统语言相同\n"
        "{}={}",
        CFG_LANG_ZH, CFG_LANG_EN, CFG_KEY_LANG, CFG_LANG_ZH);

    std::string mode = std::format(
        "# 可选项： \n"
        "# - {} ：（默认）显示菜单，可以点击要做的操作\n"
        "# - {} ：延迟之后直接关机\n"
        "{}={}",
        CFG_ACTION_NONE, CFG_ACTION_SOME, CFG_KEY_ACTION, CFG_ACTION_NONE);

    std::string instruction = std::format(
        "# 可选项：{}, {}（默认）\n"
        "{}={}",
        CFG_INSTRUCTION_HIDDEN, CFG_INSTRUCTION_SHOW, CFG_KEY_INSTRUCTION, CFG_INSTRUCTION_SHOW);

    std::string delay = std::format(
        "# 在执行操作之前等待这么多秒，默认值：{}\n"
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
        "# Wait time (in seconds) before action. Default: {}\n"
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
      delay(CFG_DEFAULT_DELAY),
      instruction(Instruction::Show) {
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

void Config::Load() {
    std::wstring configPath = this->GetConfigPath();
    std::ifstream file(configPath);
    if (!file.is_open()) {
        std::string content = IsSysLangChinese() ? DefaultConfigZh() : DefaultConfigEn();
        std::ofstream out(configPath);
        out << content;
        out.close();
        return;
    }

    std::string line;

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
            this->lang = value == CFG_LANG_EN ? Lang::En : Lang::Zh;
        } else if (key == CFG_KEY_ACTION) {
            if (value == CFG_ACTION_NONE) {
                this->action = Action::None;
            } else if (value == CFG_ACTION_SLEEP) {
                this->action = Action::Sleep;
            } else if (value == CFG_ACTION_SHUTDOWN) {
                this->action = Action::Shutdown;
            } else if (value == CFG_ACTION_RESTART) {
                this->action = Action::Restart;
            } else if (value == CFG_ACTION_LOCK) {
                this->action = Action::Lock;
            } else {
                this->action = Action::None;
            }
        } else if (key == CFG_KEY_INSTRUCTION) {
            this->instruction =
                value == CFG_INSTRUCTION_SHOW ? Instruction::Show : Instruction::Hidden;
        } else if (key == CFG_KEY_DELAY) {
            try {
                this->delay = std::clamp(std::stoi(value), 0, 60);
            } catch (const std::invalid_argument&) {
                this->delay = CFG_DEFAULT_DELAY;
            }
        }
    }
}