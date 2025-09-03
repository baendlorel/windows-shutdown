#include "config.h"

#include <windows.h>

#include <algorithm>
#include <format>
#include <fstream>
#include <sstream>

std::string DefaultConfigZh() {
    // Create default config.ini
    std::string lang = std::format(
        "# '{}' ��ѡ� '{}', '{}'. Ĭ��ֵ��ϵͳ������ͬ\n"
        "{}={}",
        CFG_KEY_LANG, CFG_LANG_ZH, CFG_LANG_EN, CFG_KEY_LANG, CFG_LANG_ZH);

    std::string mode = std::format(
        "# '{}' ��ѡ� ��Ĭ��ֵ�� {}��\n"
        "# - '{}' ����ʾ�˵������Ե��Ҫ���Ĳ���\n"
        "# - '{}' ���ӳ�֮��ֱ�ӹػ�\n"
        "{}={}",
        CFG_KEY_MODE, CFG_MODE_NORMAL, CFG_MODE_NORMAL, CFG_MODE_IMMEDIATE, CFG_KEY_MODE,
        CFG_MODE_NORMAL);

    std::string instruction = std::format(
        "# '{}' ��ѡ�'{}', '{}'. Ĭ��ֵ��'{}'\n"
        "{}={}",
        CFG_KEY_INSTRUCTION, CFG_INSTRUCTION_SHOW, CFG_INSTRUCTION_HIDDEN, CFG_INSTRUCTION_SHOW,
        CFG_KEY_INSTRUCTION, CFG_INSTRUCTION_SHOW);

    std::string delay = std::format(
        "# ��ִ�в���֮ǰ�ȴ���ô���룬Ĭ��ֵ��{}\n"
        "{}={}",
        CFG_DEFAULT_DELAY, CFG_KEY_DELAY, CFG_DEFAULT_DELAY);

    return std::format(
        "# ��������ʧ��ʱ��ʹ��Ĭ�����á�\n"
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
        "# '{}' options: '{}', '{}'. Default: same as your system\n"
        "{}={}",
        CFG_KEY_LANG, CFG_LANG_ZH, CFG_LANG_EN, CFG_KEY_LANG, CFG_LANG_ZH);

    std::string mode = std::format(
        "# '{}' options: (Default: {})\n"
        "# - '{}' : show menu to choose an action\n"
        "# - '{}' : after delayed seconds, shutdown instantly\n"
        "{}={}",
        CFG_KEY_MODE, CFG_MODE_NORMAL, CFG_MODE_NORMAL, CFG_MODE_IMMEDIATE, CFG_KEY_MODE,
        CFG_MODE_NORMAL);

    std::string instruction = std::format(
        "# '{}' options: '{}', '{}'. Default: '{}'\n"
        "{}={}",
        CFG_KEY_INSTRUCTION, CFG_INSTRUCTION_SHOW, CFG_INSTRUCTION_HIDDEN, CFG_INSTRUCTION_SHOW,
        CFG_KEY_INSTRUCTION, CFG_INSTRUCTION_SHOW);

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
      mode(Mode::Normal),
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

    enum {
        ConfigLang,
        ConfigMode,
        ConfigInstruction,
        ConfigDelay,
    };

    bool isSet[4] = {false, false, false, false};

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
            isSet[ConfigLang] = true;
        } else if (key == CFG_KEY_MODE) {
            this->mode = value == CFG_MODE_IMMEDIATE ? Mode::Immediate : Mode::Normal;
            isSet[ConfigMode] = true;
        } else if (key == CFG_KEY_INSTRUCTION) {
            this->instruction =
                value == CFG_INSTRUCTION_SHOW ? Instruction::Show : Instruction::Hidden;
            isSet[ConfigInstruction] = true;
        } else if (key == CFG_KEY_DELAY) {
            try {
                this->delay = std::clamp(std::stoi(value), 0, 60);
            } catch (const std::invalid_argument&) {
                this->delay = CFG_DEFAULT_DELAY;
            }
            isSet[ConfigDelay] = true;
        }
    }

    if (!isSet[ConfigLang]) {
        this->lang = IsSysLangChinese() ? Lang::Zh : Lang::En;
    }
    if (!isSet[ConfigMode]) {
        this->mode = Mode::Normal;
    }
    if (!isSet[ConfigInstruction]) {
        this->instruction = Instruction::Show;
    }
    if (!isSet[ConfigDelay]) {
        this->delay = CFG_DEFAULT_DELAY;
    }
}