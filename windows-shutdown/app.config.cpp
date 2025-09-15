#include "app.config.h"

#include <format>
#include <sstream>
#include <fstream>

#include "realify.h"
#include "style.color.h"
#include "utils.string.h"
#include "utils.fs.h"

namespace {
std::string default_content_zh() {
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

std::string default_content_en() {
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

bool is_sys_lang_chinese() {
    const LANGID langId = GetUserDefaultUILanguage();
    const WORD primaryLang = PRIMARYLANGID(langId);
    return primaryLang == LANG_CHINESE;
}

// Use this in case of returning to MSVC
std::string wstring_to_utf8(const std::wstring& s) {
    if (s.empty()) return {};
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, s.data(), static_cast<int>(s.size()), nullptr,
                                          0, nullptr, nullptr);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, s.data(), static_cast<int>(s.size()), strTo.data(), size_needed,
                        nullptr, nullptr);
    return strTo;
}

// Helper function to parse Action from string (case-insensitive)
app::Action parse_action_from_string(const std::string& actionStr) {
    std::string lowerStr = to_lowercase(actionStr);

    if (lowerStr == cfg::MENU_BUTTON_DONATE) {
        return app::Action::Donate;
    }
    if (lowerStr == cfg::MENU_BUTTON_CONFIG) {
        return app::Action::Config;
    }
    if (lowerStr == cfg::MENU_BUTTON_LOCK) {
        return app::Action::Lock;
    }
    if (lowerStr == cfg::MENU_BUTTON_SLEEP) {
        return app::Action::Sleep;
    }
    if (lowerStr == cfg::MENU_BUTTON_RESTART) {
        return app::Action::Restart;
    }
    if (lowerStr == cfg::MENU_BUTTON_SHUTDOWN) {
        return app::Action::Shutdown;
    }
    return app::Action::None;  // Invalid action
}
}  // namespace

std::wstring AppConfig::get_config_path() {
    wchar_t exePath[MAX_PATH] = {};
    GetModuleFileNameW(nullptr, exePath, MAX_PATH);
    std::wstring path(exePath);
    const auto pos = path.find_last_of(L"\\/");
    if (pos != std::wstring::npos) {
        path = path.substr(0, pos + 1);
    } else {
        path = L"";
    }
    path += cfg::FILE_NAME;
    return path;
}

// key and value is already lowercase
warning::Code AppConfig::load_key_value(const std::string& key, const std::string& value) {
    if (key == cfg::LOWER_KEY_LANG) {
        if (value == cfg::LANG_EN) {
            this->lang = cfg::Lang::En;
            return warning::Code::None;
        }

        if (value == cfg::LANG_ZH) {
            this->lang = cfg::Lang::Zh;
            return warning::Code::None;
        }

        return warning::Code::InvalidLanguage;
    }

    if (key == cfg::LOWER_KEY_ACTION) {
        if (value == cfg::ACTION_NONE) {
            this->action = app::Action::None;
            return warning::Code::None;
        }
        if (value == cfg::ACTION_SLEEP) {
            this->action = app::Action::Sleep;
            return warning::Code::None;
        }
        if (value == cfg::ACTION_SHUTDOWN) {
            this->action = app::Action::Shutdown;
            return warning::Code::None;
        }
        if (value == cfg::ACTION_RESTART) {
            this->action = app::Action::Restart;
            return warning::Code::None;
        }
        if (value == cfg::ACTION_LOCK) {
            this->action = app::Action::Lock;
            return warning::Code::None;
        }
        this->action = app::Action::None;
        return warning::Code::InvalidAction;
    }

    if (key == cfg::LOWER_KEY_INSTRUCTION) {
        if (value == cfg::INSTRUCTION_SHOW) {
            this->instruction = cfg::Instruction::Show;
            return warning::Code::None;
        }

        if (value == cfg::INSTRUCTION_HIDE) {
            this->instruction = cfg::Instruction::Hide;
            return warning::Code::None;
        }

        return warning::Code::InvalidInstruction;
    }

    if (key == cfg::LOWER_KEY_MENU_BUTTONS) {
        if (value.empty()) {
            return warning::Code::InvalidMenuButton;
        }

        std::vector<app::Action> buttons;
        std::stringstream ss(value);
        std::string item;
        bool hasInvalidItems = false;

        while (std::getline(ss, item, ',')) {
            // Remove leading and trailing spaces
            item.erase(0, item.find_first_not_of(" \t"));
            item.erase(item.find_last_not_of(" \t") + 1);

            if (!item.empty()) {
                app::Action item_action = parse_action_from_string(item);
                if (item_action == app::Action::None) {
                    // Invalid menu button name, mark as having invalid items
                    hasInvalidItems = true;
                    continue;
                }
                buttons.push_back(item_action);
            }
        }

        // If no valid buttons were parsed, use default
        if (buttons.empty()) {
            return warning::Code::InvalidMenuButton;
        }

        this->menu_buttons = buttons;

        // Return warning if we had invalid items but some valid ones
        return hasInvalidItems ? warning::Code::InvalidMenuButton : warning::Code::None;
    }

    if (key == cfg::LOWER_KEY_COUNTDOWN_STYLE) {
        if (value == cfg::COUNTDOWN_STYLE_NORMAL) {
            this->countdown_style = cfg::CountdownStyle::Normal;
            return warning::Code::None;
        }

        if (value == cfg::COUNTDOWN_STYLE_STEINS_GATE) {
            this->countdown_style = cfg::CountdownStyle::SteinsGate;
            return warning::Code::None;
        }

        return warning::Code::InvalidCountdownStyle;
    }

    if (key == cfg::LOWER_KEY_DELAY) {
        try {
            int num = std::stoi(value);
            if (num < 0) {
                this->delay = cfg::CFG_DEFAULT_DELAY;
                return warning::Code::InvalidDelay;
            }
            this->delay = num;
            return warning::Code::None;
        } catch (...) {
            this->delay = cfg::CFG_DEFAULT_DELAY;
            return warning::Code::InvalidDelay;
        }
    }

    if (key == cfg::LOWER_KEY_BACKGROUND_COLOR) {
        if (value.size() == 9 && value[0] == '#') {
            try {
                const BYTE r = to_byte(std::stoi(value.substr(1, 2), nullptr, 16));
                const BYTE g = to_byte(std::stoi(value.substr(3, 2), nullptr, 16));
                const BYTE b = to_byte(std::stoi(value.substr(5, 2), nullptr, 16));
                const BYTE a = to_byte(std::stoi(value.substr(7, 2), nullptr, 16));
                this->background_color = Gdiplus::Color(a, r, g, b);
                return warning::Code::None;
            } catch (...) {
                return warning::Code::InvalidBackgroundColorValue;
            }
        }

        if (value.size() == 7 && value[0] == '#') {
            try {
                const BYTE r = to_byte(std::stoi(value.substr(1, 2), nullptr, 16));
                const BYTE g = to_byte(std::stoi(value.substr(3, 2), nullptr, 16));
                const BYTE b = to_byte(std::stoi(value.substr(5, 2), nullptr, 16));
                this->background_color = Gdiplus::Color(color_set::DEFAULT_ALPHA, r, g, b);
                return warning::Code::None;
            } catch (...) {
                return warning::Code::InvalidBackgroundColorValue;
            }
        }

        return warning::Code::InvalidBackgroundColorFormat;
    }

    return warning::Code::UnknownConfigKey;
}

void AppConfig::load() {
    const std::wstring configPathW = get_config_path();
    const std::string configPath = wstring_to_utf8(configPathW);

    // Read config file as UTF-8
    const std::ifstream file(configPath);
    if (!file.is_open()) {
        const std::string content =
            is_sys_lang_chinese() ? default_content_zh() : default_content_en();
        // std::ofstream out(configPath);
        // const unsigned char bom[] = {0xEF, 0xBB, 0xBF};
        // out.write((const char*)bom, sizeof(bom));
        // out << content;
        // out.close();
        fs::save_content(configPath, content);
        return;
    }

    const std::string content = fs::read_content(configPath);

    // Parse config file line by line (UTF-8)

    short line_no = 0;

    fs::for_each_line(content, [this, &line_no](std::string line) {
        line_no++;
        if (line.empty()) {
            return;
        }
        line = trim(line);
        if (line.empty() || line[0] == '#') {
            return;
        }
        const auto eq = line.find('=');
        if (eq == std::string::npos) {
            return;
        }

        const std::string key = to_lowercase(trim(line.substr(0, eq)));
        const std::string value = to_lowercase(trim(line.substr(eq + 1)));

        auto warning_code = this->load_key_value(key, value);
        if (warning_code != warning::Code::None) {
            this->warnings.emplace_back(warning_code, line_no);
        }
    });
}
