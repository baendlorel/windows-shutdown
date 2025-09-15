#include "app.config.h"

#include <format>
#include <sstream>
#include <fstream>

#include "style.color.h"
#include "utils.string.h"
#include "utils.fs.h"

std::string DefaultConfigZh() {
    // Create default
    std::string lang = std::format(
        "# 可填：{}, {}。 默认值和系统语言相同\n"
        "{}={}",
        CFG::LANG_ZH, CFG::LANG_EN, CFG::KEY_LANG, CFG::LANG_ZH);

    std::string action = std::format(
        "# 可填： \n"
        "# - {}（默认）：显示菜单，可以点击要做的操作\n"
        "# - {}：延迟之后直接执行操作\n"
        "{}={}",
        CFG::ACTION_NONE, CFG::ACTION_SOME, CFG::KEY_ACTION, CFG::ACTION_NONE);

    std::string instruction = std::format(
        "# 可填：{}, {}（默认）\n"
        "{}={}",
        CFG::INSTRUCTION_HIDE, CFG::INSTRUCTION_SHOW, CFG::KEY_INSTRUCTION, CFG::INSTRUCTION_SHOW);

    std::string menuButtons = std::format(
        "# 菜单按钮配置，以英文逗号分隔，可填：{}\n"
        "{}={}",
        CFG::MENU_BUTTONS_SOME, CFG::KEY_MENU_BUTTONS, CFG::MENU_BUTTONS_SOME);

    std::string countdownStyle = std::format(
        "# 倒计时风格：{}, {}（默认）\n"
        "{}={}",
        CFG::COUNTDOWN_STYLE_NORMAL, CFG::COUNTDOWN_STYLE_STEINS_GATE, CFG::KEY_COUNTDOWN_STYLE,
        CFG::COUNTDOWN_STYLE_STEINS_GATE);

    std::string delay = std::format(
        "# 在执行操作之前等待这么多秒，默认{}秒\n"
        "{}={}",
        CFG::CFG_DEFAULT_DELAY, CFG::KEY_DELAY, CFG::CFG_DEFAULT_DELAY);

    std::string bgColor = std::format(
        "# 背景色，格式为#RRGGBBAA或#RRGGBB，默认黑色半透明\n"
        "{}={}",
        CFG::KEY_BACKGROUND_COLOR, CFG::BACKGROUND_COLOR_DEFAULT);

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
        "{}\n",
        lang, action, delay, instruction, menuButtons, countdownStyle, bgColor);
}

std::string DefaultConfigEn() {
    std::string lang = std::format(
        "# Options: {}, {}. Default: same as your system\n"
        "{}={}",
        CFG::LANG_ZH, CFG::LANG_EN, CFG::KEY_LANG, CFG::LANG_ZH);

    std::string action = std::format(
        "# Options: \n"
        "# - {} : (Default)show menu to choose an action\n"
        "# - {} : After delayed seconds, do it instantly\n"
        "{}={}",
        CFG::ACTION_NONE, CFG::ACTION_SOME, CFG::KEY_ACTION, CFG::ACTION_NONE);

    std::string instruction = std::format(
        "# Options: {}, {}(Default)\n"
        "{}={}",
        CFG::INSTRUCTION_HIDE, CFG::INSTRUCTION_SHOW, CFG::KEY_INSTRUCTION, CFG::INSTRUCTION_SHOW);

    std::string menuButtons = std::format(
        "# Menu buttons configuration, comma separated. Options: {}\n"
        "{}={}",
        CFG::MENU_BUTTONS_SOME, CFG::KEY_MENU_BUTTONS, CFG::MENU_BUTTONS_SOME);

    std::string countdownStyle = std::format(
        "# Countdown style: {}, {}(Default)\n"
        "{}={}",
        CFG::COUNTDOWN_STYLE_NORMAL, CFG::COUNTDOWN_STYLE_STEINS_GATE, CFG::KEY_COUNTDOWN_STYLE,
        CFG::COUNTDOWN_STYLE_STEINS_GATE);

    std::string delay = std::format(
        "# Wait time (in seconds, default is {}s) before action.\n"
        "{}={}",
        CFG::CFG_DEFAULT_DELAY, CFG::KEY_DELAY, CFG::CFG_DEFAULT_DELAY);

    std::string bgColor = std::format(
        "# Background color, format: #RRGGBBAA or #RRGGBB, default is black semi-transparent\n"
        "{}={}",
        CFG::KEY_BACKGROUND_COLOR, CFG::BACKGROUND_COLOR_DEFAULT);

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
        "{}\n",
        lang, action, delay, instruction, menuButtons, countdownStyle, bgColor);
}

bool IsSysLangChinese() {
    LANGID langId = GetUserDefaultUILanguage();
    WORD primaryLang = PRIMARYLANGID(langId);
    return primaryLang == LANG_CHINESE;
}

std::wstring AppConfig::get_config_path() {
    wchar_t exePath[MAX_PATH] = {0};
    GetModuleFileNameW(NULL, exePath, MAX_PATH);
    std::wstring path(exePath);
    auto pos = path.find_last_of(L"\\/");
    if (pos != std::wstring::npos) {
        path = path.substr(0, pos + 1);
    } else {
        path = L"";
    }
    path += CFG::FILE_NAME;
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
    std::string lowerStr = to_lowercase(actionStr);

    if (lowerStr == CFG::MENU_BUTTON_DONATE) {
        return Action::Donate;
    }
    if (lowerStr == CFG::MENU_BUTTON_CONFIG) {
        return Action::Config;
    }
    if (lowerStr == CFG::MENU_BUTTON_LOCK) {
        return Action::Lock;
    }
    if (lowerStr == CFG::MENU_BUTTON_SLEEP) {
        return Action::Sleep;
    }
    if (lowerStr == CFG::MENU_BUTTON_RESTART) {
        return Action::Restart;
    }
    if (lowerStr == CFG::MENU_BUTTON_SHUTDOWN) {
        return Action::Shutdown;
    }
    return Action::None;  // Invalid action
}

// key and value is already lowercase
CFG::Warning AppConfig::load_key_value(std::string& key, std::string& value) {
    if (key == CFG::LOWER_KEY_LANG) {
        if (value == CFG::LANG_EN) {
            this->lang = CFG::Lang::En;
            return CFG::Warning::None;
        }

        if (value == CFG::LANG_ZH) {
            this->lang = CFG::Lang::Zh;
            return CFG::Warning::None;
        }

        return CFG::Warning::InvalidLanguage;
    }

    if (key == CFG::LOWER_KEY_ACTION) {
        if (value == CFG::ACTION_NONE) {
            this->action = Action::None;
            return CFG::Warning::None;
        }
        if (value == CFG::ACTION_SLEEP) {
            this->action = Action::Sleep;
            return CFG::Warning::None;
        }
        if (value == CFG::ACTION_SHUTDOWN) {
            this->action = Action::Shutdown;
            return CFG::Warning::None;
        }
        if (value == CFG::ACTION_RESTART) {
            this->action = Action::Restart;
            return CFG::Warning::None;
        }
        if (value == CFG::ACTION_LOCK) {
            this->action = Action::Lock;
            return CFG::Warning::None;
        }
        this->action = Action::None;
        return CFG::Warning::InvalidAction;
    }

    if (key == CFG::LOWER_KEY_INSTRUCTION) {
        if (value == CFG::INSTRUCTION_SHOW) {
            this->instruction = CFG::Instruction::Show;
            return CFG::Warning::None;
        }

        if (value == CFG::INSTRUCTION_HIDE) {
            this->instruction = CFG::Instruction::Hide;
            return CFG::Warning::None;
        }

        return CFG::Warning::InvalidInstruction;
    }

    if (key == CFG::LOWER_KEY_MENU_BUTTONS) {
        if (value.empty()) {
            return CFG::Warning::InvalidMenuButton;
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
            return CFG::Warning::InvalidMenuButton;
        } else {
            this->menu_buttons = buttons;
        }

        // Return warning if we had invalid items but some valid ones
        return hasInvalidItems ? CFG::Warning::InvalidMenuButton : CFG::Warning::None;
    }

    if (key == CFG::LOWER_KEY_COUNTDOWN_STYLE) {
        if (value == CFG::COUNTDOWN_STYLE_NORMAL) {
            this->countdown_style = CFG::CountdownStyle::Normal;
            return CFG::Warning::None;
        }

        if (value == CFG::COUNTDOWN_STYLE_STEINS_GATE) {
            this->countdown_style = CFG::CountdownStyle::SteinsGate;
            return CFG::Warning::None;
        }

        return CFG::Warning::InvalidCountdownStyle;
    }

    if (key == CFG::LOWER_KEY_DELAY) {
        try {
            int num = std::stoi(value);
            if (num < 0) {
                this->delay = CFG::CFG_DEFAULT_DELAY;
                return CFG::Warning::InvalidDelay;
            }
            this->delay = num;
            return CFG::Warning::None;
        } catch (...) {
            this->delay = CFG::CFG_DEFAULT_DELAY;
            return CFG::Warning::InvalidDelay;
        }
    }

    if (key == CFG::LOWER_KEY_BACKGROUND_COLOR) {
        if (value.size() == 9 && value[0] == '#') {
            try {
                BYTE r = std::stoi(value.substr(1, 2), nullptr, 16);
                BYTE g = std::stoi(value.substr(3, 2), nullptr, 16);
                BYTE b = std::stoi(value.substr(5, 2), nullptr, 16);
                BYTE a = std::stoi(value.substr(7, 2), nullptr, 16);
                this->background_color = Gdiplus::Color(a, r, g, b);
                return CFG::Warning::None;
            } catch (...) {
                return CFG::Warning::InvalidBackgroundColorValue;
            }
        }

        if (value.size() == 7 && value[0] == '#') {
            try {
                BYTE r = std::stoi(value.substr(1, 2), nullptr, 16);
                BYTE g = std::stoi(value.substr(3, 2), nullptr, 16);
                BYTE b = std::stoi(value.substr(5, 2), nullptr, 16);
                this->background_color = Gdiplus::Color(DEFAULT_ALPHA, r, g, b);
                return CFG::Warning::None;
            } catch (...) {
                return CFG::Warning::InvalidBackgroundColorValue;
            }
        }

        return CFG::Warning::InvalidBackgroundColorFormat;
    }

    return CFG::Warning::UnknownConfigKey;
}

void AppConfig::load() {
    std::wstring configPathW = this->get_config_path();
    std::string configPath = WStringToUtf8(configPathW);

    // Read config file as UTF-8
    std::ifstream file(configPath);
    if (!file.is_open()) {
        std::string content = IsSysLangChinese() ? DefaultConfigZh() : DefaultConfigEn();
        // std::ofstream out(configPath);
        // const unsigned char bom[] = {0xEF, 0xBB, 0xBF};
        // out.write((const char*)bom, sizeof(bom));
        // out << content;
        // out.close();
        save_to_file(configPath, content);
        return;
    }

    std::string content = read_file_content(configPath);

    // Parse config file line by line (UTF-8)

    short lineNo = 0;

    for_each_line(content, [this, &lineNo](std::string line) {
        lineNo++;
        if (line.empty()) {
            return;
        }
        line = trim(line);
        if (line.empty() || line[0] == '#') {
            return;
        }
        auto eq = line.find('=');
        if (eq == std::string::npos) {
            return;
        }

        std::string key = to_lowercase(trim(line.substr(0, eq)));
        std::string value = to_lowercase(trim(line.substr(eq + 1)));

        auto warning = this->load_key_value(key, value);
        if (warning != CFG::Warning::None) {
            this->warnings.push_back({.warning = warning, .lineNo = lineNo});
        }
    });
}
