#include "config.h"

#include <windows.h>
#include <format>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

#include "consts/color-set.h"

std::string DefaultConfigZh() {
    // Create default
    std::string lang = std::format(
        "# 可填：{}, {}。 默认值和系统语言相同\n"
        "{}={}",
        CFG_LANG_ZH, CFG_LANG_EN, CFG_KEY_LANG, CFG_LANG_ZH);

    std::string action = std::format(
        "# 可填： \n"
        "# - {}（默认）：显示菜单，可以点击要做的操作\n"
        "# - {}：延迟之后直接执行操作\n"
        "{}={}",
        CFG_ACTION_NONE, CFG_ACTION_SOME, CFG_KEY_ACTION, CFG_ACTION_NONE);

    std::string instruction = std::format(
        "# 可填：{}, {}（默认）\n"
        "{}={}",
        CFG_INSTRUCTION_HIDE, CFG_INSTRUCTION_SHOW, CFG_KEY_INSTRUCTION, CFG_INSTRUCTION_SHOW);

    std::string menuButtons = std::format(
        "# 菜单按钮配置，以英文逗号分隔，可填：{}\n"
        "{}={}",
        CFG_MENU_BUTTONS_SOME, CFG_KEY_MENU_BUTTONS, CFG_MENU_BUTTONS_SOME);

    std::string countdownStyle = std::format(
        "# 倒计时风格：{}, {}（默认）\n"
        "{}={}",
        CFG_COUNTDOWN_STYLE_NORMAL, CFG_COUNTDOWN_STYLE_STEINS_GATE, CFG_KEY_COUNTDOWN_STYLE,
        CFG_COUNTDOWN_STYLE_STEINS_GATE);

    std::string delay = std::format(
        "# 在执行操作之前等待这么多秒，默认{}秒\n"
        "{}={}",
        CFG_DEFAULT_DELAY, CFG_KEY_DELAY, CFG_DEFAULT_DELAY);

    std::string bgColor = std::format(
        "# 背景色，格式为#RRGGBBAA或#RRGGBB，默认黑色半透明\n"
        "{}={}",
        CFG_KEY_BACKGROUND_COLOR, CFG_BACKGROUND_COLOR_DEFAULT);

    return std::format(
        "# 加载配置失败时会使用默认配置。\n"
        "\n"
        "{}\n"
        "\n"
        "{}\n"
        "\n"
        "{}\n"
        "\n"
        "{}\n"
        "\n"
        "{}\n"
        "\n"
        "{}\n"
        "\n"
        "{}\n",
        lang, action, delay, instruction, menuButtons, countdownStyle, bgColor);
}

std::string DefaultConfigEn() {
    std::string lang = std::format(
        "# Options: {}, {}. Default: same as your system\n"
        "{}={}",
        CFG_LANG_ZH, CFG_LANG_EN, CFG_KEY_LANG, CFG_LANG_ZH);

    std::string action = std::format(
        "# Options: \n"
        "# - {} : (Default)show menu to choose an action\n"
        "# - {} : After delayed seconds, do it instantly\n"
        "{}={}",
        CFG_ACTION_NONE, CFG_ACTION_SOME, CFG_KEY_ACTION, CFG_ACTION_NONE);

    std::string instruction = std::format(
        "# Options: {}, {}(Default)\n"
        "{}={}",
        CFG_INSTRUCTION_HIDE, CFG_INSTRUCTION_SHOW, CFG_KEY_INSTRUCTION, CFG_INSTRUCTION_SHOW);

    std::string menuButtons = std::format(
        "# Menu buttons configuration, comma separated. Options: {}\n"
        "{}={}",
        CFG_MENU_BUTTONS_SOME, CFG_KEY_MENU_BUTTONS, CFG_MENU_BUTTONS_SOME);

    std::string countdownStyle = std::format(
        "# Countdown style: {}, {}(Default)\n"
        "{}={}",
        CFG_COUNTDOWN_STYLE_NORMAL, CFG_COUNTDOWN_STYLE_STEINS_GATE, CFG_KEY_COUNTDOWN_STYLE,
        CFG_COUNTDOWN_STYLE_STEINS_GATE);

    std::string delay = std::format(
        "# Wait time (in seconds, default is {}s) before action.\n"
        "{}={}",
        CFG_DEFAULT_DELAY, CFG_KEY_DELAY, CFG_DEFAULT_DELAY);

    std::string bgColor = std::format(
        "# Background color, format: #RRGGBBAA or #RRGGBB, default is black semi-transparent\n"
        "{}={}",
        CFG_KEY_BACKGROUND_COLOR, CFG_BACKGROUND_COLOR_DEFAULT);

    return std::format(
        "# Default configuration will be used if loading config fails.\n"
        "\n"
        "{}\n"
        "\n"
        "{}\n"
        "\n"
        "{}\n"
        "\n"
        "{}\n"
        "\n"
        "{}\n"
        "\n"
        "{}\n"
        "\n"
        "{}\n",
        lang, action, delay, instruction, menuButtons, countdownStyle, bgColor);
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

// Convert string to lowercase for case-insensitive comparison
std::string toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
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
      menuButtons({Action::Donate, Action::Config, Action::Lock, Action::Sleep, Action::Restart,
                   Action::Shutdown}),
      countdownStyle(CountdownStyle::SteinsGate),
      delay(CFG_DEFAULT_DELAY),
      backgroundColor(ColorSet::GetInstance().BackgroundColor) {
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
    path += CFG_FILE_NAME;
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

// Helper function to parse Action from string (case-insensitive)
Action ParseActionFromString(const std::string& actionStr) {
    std::string lowerStr = toLower(actionStr);

    if (lowerStr == CFG_MENU_BUTTON_DONATE) {
        return Action::Donate;
    }
    if (lowerStr == CFG_MENU_BUTTON_CONFIG) {
        return Action::Config;
    }
    if (lowerStr == CFG_MENU_BUTTON_LOCK) {
        return Action::Lock;
    }
    if (lowerStr == CFG_MENU_BUTTON_SLEEP) {
        return Action::Sleep;
    }
    if (lowerStr == CFG_MENU_BUTTON_RESTART) {
        return Action::Restart;
    }
    if (lowerStr == CFG_MENU_BUTTON_SHUTDOWN) {
        return Action::Shutdown;
    }
    return Action::None;  // Invalid action
}

ConfigWarning Config::LoadKeyValue(std::string& key, std::string& value) {
    if (key == CFG_KEY_LANG) {
        std::string valLower = toLower(value);
        if (valLower == CFG_LANG_EN) {
            this->lang = Lang::En;
            return ConfigWarning::None;
        }

        if (valLower == CFG_LANG_ZH) {
            this->lang = Lang::Zh;
            return ConfigWarning::None;
        }

        return ConfigWarning::InvalidLanguage;
    }

    if (key == CFG_KEY_ACTION) {
        std::string valLower = toLower(value);
        if (valLower == CFG_ACTION_NONE) {
            this->action = Action::None;
            return ConfigWarning::None;
        }
        if (valLower == CFG_ACTION_SLEEP) {
            this->action = Action::Sleep;
            return ConfigWarning::None;
        }
        if (valLower == CFG_ACTION_SHUTDOWN) {
            this->action = Action::Shutdown;
            return ConfigWarning::None;
        }
        if (valLower == CFG_ACTION_RESTART) {
            this->action = Action::Restart;
            return ConfigWarning::None;
        }
        if (valLower == CFG_ACTION_LOCK) {
            this->action = Action::Lock;
            return ConfigWarning::None;
        }
        this->action = Action::None;
        return ConfigWarning::InvalidAction;
    }

    if (key == CFG_KEY_INSTRUCTION) {
        std::string valLower = toLower(value);
        if (valLower == CFG_INSTRUCTION_SHOW) {
            this->instruction = Instruction::Show;
            return ConfigWarning::None;
        }

        if (valLower == CFG_INSTRUCTION_HIDE) {
            this->instruction = Instruction::Hide;
            return ConfigWarning::None;
        }

        return ConfigWarning::InvalidInstruction;
    }

    if (key == CFG_KEY_MENU_BUTTONS) {
        if (value.empty()) {
            return ConfigWarning::InvalidMenuButton;
        }

        std::vector<Action> buttons;
        std::stringstream ss(value);
        std::string item;
        bool hasInvalidItems = false;

        while (std::getline(ss, item, ',')) {
            // Remove leading and trailing spaces
            item.erase(0, item.find_first_not_of(" \t"));
            item.erase(item.find_last_not_of(" \t") + 1);

            if (!item.empty()) {
                Action action = ParseActionFromString(item);
                if (action == Action::None) {
                    // Invalid menu button name, mark as having invalid items
                    hasInvalidItems = true;
                    continue;
                }
                buttons.push_back(action);
            }
        }

        // If no valid buttons were parsed, use default
        if (buttons.empty()) {
            return ConfigWarning::InvalidMenuButton;
        } else {
            this->menuButtons = buttons;
        }

        // Return warning if we had invalid items but some valid ones
        return hasInvalidItems ? ConfigWarning::InvalidMenuButton : ConfigWarning::None;
    }

    if (key == CFG_KEY_COUNTDOWN_STYLE) {
        std::string valLower = toLower(value);
        if (valLower == CFG_COUNTDOWN_STYLE_NORMAL) {
            this->countdownStyle = CountdownStyle::Normal;
            return ConfigWarning::None;
        }

        if (valLower == CFG_COUNTDOWN_STYLE_STEINS_GATE) {
            this->countdownStyle = CountdownStyle::SteinsGate;
            return ConfigWarning::None;
        }

        return ConfigWarning::InvalidCountdownStyle;
    }

    if (key == CFG_KEY_DELAY) {
        try {
            int num = std::stoi(value);
            if (num < 0) {
                this->delay = CFG_DEFAULT_DELAY;
                return ConfigWarning::InvalidDelay;
            }
            this->delay = num;
            return ConfigWarning::None;
        } catch (...) {
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
                this->backgroundColor = Gdiplus::Color(a, r, g, b);
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
                this->backgroundColor = Gdiplus::Color(DEFAULT_ALPHA, r, g, b);
                return ConfigWarning::None;
            } catch (...) {
                return ConfigWarning::InvalidBackgroundColorValue;
            }
        }

        return ConfigWarning::InvalidBackgroundColorFormat;
    }

    return ConfigWarning::UnknownConfigKey;
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
    if (size > 0) {
        file.read(&buffer[0], size);
    }
    std::istringstream utf8stream(buffer);

    // Parse config file line by line (UTF-8)
    short lineNo = 0;
    std::string line;
    while (std::getline(utf8stream, line)) {
        lineNo++;
        if (line.empty()) {
            continue;
        }
        line = trim(line);
        if (line.empty() || line[0] == '#') {
            continue;
        }
        auto eq = line.find('=');
        if (eq == std::string::npos) {
            // this->warnings.push_back({.warning = ConfigWarning::NotConfigEntry, .lineNo =
            // lineNo});
            continue;
        }

        std::string key = trim(line.substr(0, eq));
        // // Extract value, ignore any inline comment starting with '#' and trim whitespace
        // std::string rawValue = line.substr(eq + 1);
        // auto commentPos = rawValue.find('#');
        // if (commentPos != std::string::npos) {
        //     rawValue = rawValue.substr(0, commentPos);
        // }
        // std::string value = trim(rawValue);
        std::string value = trim(line.substr(eq + 1));

        auto warning = this->LoadKeyValue(key, value);
        if (warning != ConfigWarning::None) {
            this->warnings.push_back({.warning = warning, .lineNo = lineNo});
        }
    }
}