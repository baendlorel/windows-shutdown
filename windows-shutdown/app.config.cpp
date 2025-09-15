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
        cfg::LANG_ZH, cfg::LANG_EN, cfg::KEY_LANG, cfg::LANG_ZH);

    std::string action = std::format(
        "# 可填： \n"
        "# - {}（默认）：显示菜单，可以点击要做的操作\n"
        "# - {}：延迟之后直接执行操作\n"
        "{}={}",
        cfg::ACTION_NONE, cfg::ACTION_SOME, cfg::KEY_ACTION, cfg::ACTION_NONE);

    std::string instruction = std::format(
        "# 可填：{}, {}（默认）\n"
        "{}={}",
        cfg::INSTRUCTION_HIDE, cfg::INSTRUCTION_SHOW, cfg::KEY_INSTRUCTION, cfg::INSTRUCTION_SHOW);

    std::string menuButtons = std::format(
        "# 菜单按钮配置，以英文逗号分隔，可填：{}\n"
        "{}={}",
        cfg::MENU_BUTTONS_SOME, cfg::KEY_MENU_BUTTONS, cfg::MENU_BUTTONS_SOME);

    std::string countdownStyle = std::format(
        "# 倒计时风格：{}, {}（默认）\n"
        "{}={}",
        cfg::COUNTDOWN_STYLE_NORMAL, cfg::COUNTDOWN_STYLE_STEINS_GATE, cfg::KEY_COUNTDOWN_STYLE,
        cfg::COUNTDOWN_STYLE_STEINS_GATE);

    std::string delay = std::format(
        "# 在执行操作之前等待这么多秒，默认{}秒\n"
        "{}={}",
        cfg::CFG_DEFAULT_DELAY, cfg::KEY_DELAY, cfg::CFG_DEFAULT_DELAY);

    std::string bgColor = std::format(
        "# 背景色，格式为#RRGGBBAA或#RRGGBB，默认黑色半透明\n"
        "{}={}",
        cfg::KEY_BACKGROUND_COLOR, cfg::BACKGROUND_COLOR_DEFAULT);

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
        cfg::LANG_ZH, cfg::LANG_EN, cfg::KEY_LANG, cfg::LANG_ZH);

    std::string action = std::format(
        "# Options: \n"
        "# - {} : (Default)show menu to choose an action\n"
        "# - {} : After delayed seconds, do it instantly\n"
        "{}={}",
        cfg::ACTION_NONE, cfg::ACTION_SOME, cfg::KEY_ACTION, cfg::ACTION_NONE);

    std::string instruction = std::format(
        "# Options: {}, {}(Default)\n"
        "{}={}",
        cfg::INSTRUCTION_HIDE, cfg::INSTRUCTION_SHOW, cfg::KEY_INSTRUCTION, cfg::INSTRUCTION_SHOW);

    std::string menuButtons = std::format(
        "# Menu buttons configuration, comma separated. Options: {}\n"
        "{}={}",
        cfg::MENU_BUTTONS_SOME, cfg::KEY_MENU_BUTTONS, cfg::MENU_BUTTONS_SOME);

    std::string countdownStyle = std::format(
        "# Countdown style: {}, {}(Default)\n"
        "{}={}",
        cfg::COUNTDOWN_STYLE_NORMAL, cfg::COUNTDOWN_STYLE_STEINS_GATE, cfg::KEY_COUNTDOWN_STYLE,
        cfg::COUNTDOWN_STYLE_STEINS_GATE);

    std::string delay = std::format(
        "# Wait time (in seconds, default is {}s) before action.\n"
        "{}={}",
        cfg::CFG_DEFAULT_DELAY, cfg::KEY_DELAY, cfg::CFG_DEFAULT_DELAY);

    std::string bgColor = std::format(
        "# Background color, format: #RRGGBBAA or #RRGGBB, default is black semi-transparent\n"
        "{}={}",
        cfg::KEY_BACKGROUND_COLOR, cfg::BACKGROUND_COLOR_DEFAULT);

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
    path += cfg::FILE_NAME;
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

    if (lowerStr == cfg::MENU_BUTTON_DONATE) {
        return Action::Donate;
    }
    if (lowerStr == cfg::MENU_BUTTON_CONFIG) {
        return Action::Config;
    }
    if (lowerStr == cfg::MENU_BUTTON_LOCK) {
        return Action::Lock;
    }
    if (lowerStr == cfg::MENU_BUTTON_SLEEP) {
        return Action::Sleep;
    }
    if (lowerStr == cfg::MENU_BUTTON_RESTART) {
        return Action::Restart;
    }
    if (lowerStr == cfg::MENU_BUTTON_SHUTDOWN) {
        return Action::Shutdown;
    }
    return Action::None;  // Invalid action
}

// key and value is already lowercase
cfg::Warning AppConfig::load_key_value(std::string& key, std::string& value) {
    if (key == cfg::LOWER_KEY_LANG) {
        if (value == cfg::LANG_EN) {
            this->lang = cfg::Lang::En;
            return cfg::Warning::None;
        }

        if (value == cfg::LANG_ZH) {
            this->lang = cfg::Lang::Zh;
            return cfg::Warning::None;
        }

        return cfg::Warning::InvalidLanguage;
    }

    if (key == cfg::LOWER_KEY_ACTION) {
        if (value == cfg::ACTION_NONE) {
            this->action = Action::None;
            return cfg::Warning::None;
        }
        if (value == cfg::ACTION_SLEEP) {
            this->action = Action::Sleep;
            return cfg::Warning::None;
        }
        if (value == cfg::ACTION_SHUTDOWN) {
            this->action = Action::Shutdown;
            return cfg::Warning::None;
        }
        if (value == cfg::ACTION_RESTART) {
            this->action = Action::Restart;
            return cfg::Warning::None;
        }
        if (value == cfg::ACTION_LOCK) {
            this->action = Action::Lock;
            return cfg::Warning::None;
        }
        this->action = Action::None;
        return cfg::Warning::InvalidAction;
    }

    if (key == cfg::LOWER_KEY_INSTRUCTION) {
        if (value == cfg::INSTRUCTION_SHOW) {
            this->instruction = cfg::Instruction::Show;
            return cfg::Warning::None;
        }

        if (value == cfg::INSTRUCTION_HIDE) {
            this->instruction = cfg::Instruction::Hide;
            return cfg::Warning::None;
        }

        return cfg::Warning::InvalidInstruction;
    }

    if (key == cfg::LOWER_KEY_MENU_BUTTONS) {
        if (value.empty()) {
            return cfg::Warning::InvalidMenuButton;
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
            return cfg::Warning::InvalidMenuButton;
        } else {
            this->menu_buttons = buttons;
        }

        // Return warning if we had invalid items but some valid ones
        return hasInvalidItems ? cfg::Warning::InvalidMenuButton : cfg::Warning::None;
    }

    if (key == cfg::LOWER_KEY_COUNTDOWN_STYLE) {
        if (value == cfg::COUNTDOWN_STYLE_NORMAL) {
            this->countdown_style = cfg::CountdownStyle::Normal;
            return cfg::Warning::None;
        }

        if (value == cfg::COUNTDOWN_STYLE_STEINS_GATE) {
            this->countdown_style = cfg::CountdownStyle::SteinsGate;
            return cfg::Warning::None;
        }

        return cfg::Warning::InvalidCountdownStyle;
    }

    if (key == cfg::LOWER_KEY_DELAY) {
        try {
            int num = std::stoi(value);
            if (num < 0) {
                this->delay = cfg::CFG_DEFAULT_DELAY;
                return cfg::Warning::InvalidDelay;
            }
            this->delay = num;
            return cfg::Warning::None;
        } catch (...) {
            this->delay = cfg::CFG_DEFAULT_DELAY;
            return cfg::Warning::InvalidDelay;
        }
    }

    if (key == cfg::LOWER_KEY_BACKGROUND_COLOR) {
        if (value.size() == 9 && value[0] == '#') {
            try {
                BYTE r = std::stoi(value.substr(1, 2), nullptr, 16);
                BYTE g = std::stoi(value.substr(3, 2), nullptr, 16);
                BYTE b = std::stoi(value.substr(5, 2), nullptr, 16);
                BYTE a = std::stoi(value.substr(7, 2), nullptr, 16);
                this->background_color = Gdiplus::Color(a, r, g, b);
                return cfg::Warning::None;
            } catch (...) {
                return cfg::Warning::InvalidBackgroundColorValue;
            }
        }

        if (value.size() == 7 && value[0] == '#') {
            try {
                BYTE r = std::stoi(value.substr(1, 2), nullptr, 16);
                BYTE g = std::stoi(value.substr(3, 2), nullptr, 16);
                BYTE b = std::stoi(value.substr(5, 2), nullptr, 16);
                this->background_color = Gdiplus::Color(DEFAULT_ALPHA, r, g, b);
                return cfg::Warning::None;
            } catch (...) {
                return cfg::Warning::InvalidBackgroundColorValue;
            }
        }

        return cfg::Warning::InvalidBackgroundColorFormat;
    }

    return cfg::Warning::UnknownConfigKey;
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
        if (warning != cfg::Warning::None) {
            this->warnings.push_back({.warning = warning, .line_no = lineNo});
        }
    });
}
