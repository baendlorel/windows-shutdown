#include "i18n.h"
#include <format>

I18N& I18N::GetInstance() {
    static I18N instance;
    return instance;
}

void I18N::SetLang(Lang lang) {
    if (lang == Lang::Zh) {
        FontFamilyName = L"Microsoft YaHei UI";

        ErrCreateWindow = L"创建主窗口失败！程序无法启动。";
        ErrCreateBitmap = L"创建位图失败，无法显示主界面！";
        ErrResourceNotFound = L"找不到图标资源文件！程序可能损坏。";
        ErrLoadResource = L"加载图标资源失败！程序可能损坏。";
        ErrCreateImageStream = L"创建图像流失败！";
        ErrCreateImageBitmap = L"创建图标位图失败！";
        ErrGetProcessTokenRestart = L"获取进程权限失败，无法重启系统！";
        ErrGetProcessTokenShutdown = L"获取进程权限失败，无法关闭系统！";
        ErrLookupPrivilegeRestart = L"查找关机权限失败，无法重启系统！";
        ErrLookupPrivilegeShutdown = L"查找关机权限失败，无法关闭系统！";
        ErrRestartFailed = L"启动系统重启失败！";
        ErrShutdownFailed = L"启动系统关机失败！";
        ErrSleepFailed = L"系统睡眠失败！可能不支持睡眠功能。";
        ErrTitle = L"错误";
        PressAnyKeyToExit = L"按任意键或鼠标点击其他位置退出。";
        PressAnyKeyToCancel = L"按任意键或鼠标点击取消。";

        LineNumber = L"行号";

        // config warnings
        InvalidConfig = L"config.txt 配置文件里有无效配置：";
        UnknownConfigKey = std::format(L"[{}]是未知的配置项，已忽略。", WIDEN(CFG_KEY_LANG));
        NotConfigEntry = L"配置文件中有无法解析的行，已忽略。";
        InvalidLanguage = std::format(L"[{}]的值无效，应该是\"{},{}\"中的一个。现使用默认值{}。",
                                      WIDEN(CFG_KEY_LANG), WIDEN(CFG_LANG_ZH), WIDEN(CFG_LANG_EN),
                                      WIDEN(CFG_LANG_EN));
        InvalidAction =
            std::format(L"[{}]的值无效，应该是\"{}\"中的一个。现使用默认值{}。",
                        WIDEN(CFG_KEY_ACTION), WIDEN(CFG_ACTION_SOME), WIDEN(CFG_ACTION_NONE));
        InvalidInstruction = std::format(
            L"[{}]的值无效，应该是\"{}, {}\"中的一个。现使用默认值{}。", WIDEN(CFG_KEY_INSTRUCTION),
            WIDEN(CFG_INSTRUCTION_SHOW), WIDEN(CFG_INSTRUCTION_HIDE), WIDEN(CFG_INSTRUCTION_SHOW));
        InvalidDelay = std::format(L"[{}]必须是大于或等于0的整数。现使用默认值{}。",
                                   WIDEN(CFG_KEY_DELAY), CFG_DEFAULT_DELAY);
        InvalidBackgroundColorFormat =
            std::format(L"[{}]不是有效的#RRGGBBAA或#RRGGBB格式。现使用默认值{}。",
                        WIDEN(CFG_KEY_BACKGROUND_COLOR), WIDEN(CFG_BACKGROUND_COLOR_DEFAULT));
        InvalidBackgroundColorValue =
            std::format(L"[{}]不是有效的颜色值。现使用默认值{}。", WIDEN(CFG_KEY_BACKGROUND_COLOR),
                        WIDEN(CFG_BACKGROUND_COLOR_DEFAULT));

        // actions
        Shutdown = ZH_SHUTDOWN;
        Sleep = ZH_SLEEP;
        Restart = ZH_RESTART;
        Lock = ZH_LOCK;
        Waiting[0] = L"距离";  // e.g. "距离关机还有"
        Waiting[1] = L"还有";
        Waiting[2] = L"";  // left empty
    } else {
        FontFamilyName = L"Arial";

        ErrCreateWindow = L"Failed to create main window! The program cannot start.";
        ErrCreateBitmap = L"Failed to create bitmap, cannot display main interface!";
        ErrResourceNotFound = L"Icon resource file not found! The program may be corrupted.";
        ErrLoadResource = L"Failed to load icon resource! The program may be corrupted.";
        ErrCreateImageStream = L"Failed to create image stream!";
        ErrCreateImageBitmap = L"Failed to create icon bitmap!";
        ErrGetProcessTokenRestart = L"Failed to get process privileges, cannot restart system!";
        ErrGetProcessTokenShutdown = L"Failed to get process privileges, cannot shutdown system!";
        ErrLookupPrivilegeRestart = L"Failed to lookup shutdown privilege, cannot restart system!";
        ErrLookupPrivilegeShutdown =
            L"Failed to lookup shutdown privilege, cannot shutdown system!";
        ErrRestartFailed = L"Failed to initiate system restart!";
        ErrShutdownFailed = L"Failed to initiate system shutdown!";
        ErrSleepFailed = L"System sleep failed! Sleep function may not be supported.";
        ErrTitle = L"Error";
        PressAnyKeyToExit = L"Press any key or click elsewhere to exit.";
        PressAnyKeyToCancel = L"Press any key or click to cancel.";

        LineNumber = L"Line";

        // config warnings
        InvalidConfig = L"Configuration file has some invalid values:";
        UnknownConfigKey = L"Unknown configuration key ignored.";
        NotConfigEntry = L"Unrecognized line in configuration file ignored.";
        InvalidLanguage = std::format(
            L"[{}] is not valid, should be {} or {}. Using default value {}.", WIDEN(CFG_KEY_LANG),
            WIDEN(CFG_LANG_ZH), WIDEN(CFG_LANG_EN), WIDEN(CFG_LANG_EN));
        InvalidAction =
            std::format(L"[{}] is not valid, should be one of {}. Using default value {}.",
                        WIDEN(CFG_KEY_ACTION), WIDEN(CFG_ACTION_SOME), WIDEN(CFG_ACTION_NONE));
        InvalidInstruction =
            std::format(L"[{}] is not valid, should be {} or {}. Using default value {}.",
                        WIDEN(CFG_KEY_INSTRUCTION), WIDEN(CFG_INSTRUCTION_SHOW),
                        WIDEN(CFG_INSTRUCTION_HIDE), WIDEN(CFG_INSTRUCTION_SHOW));
        InvalidDelay = std::format(L"[{}] must be a non-negative interger. Using default value {}.",
                                   WIDEN(CFG_KEY_DELAY), CFG_DEFAULT_DELAY);
        InvalidBackgroundColorFormat =
            std::format(L"[{}] is not valid #RRGGBBAA or #RRGGBB format. Using default value {}.",
                        WIDEN(CFG_KEY_BACKGROUND_COLOR), WIDEN(CFG_BACKGROUND_COLOR_DEFAULT));
        InvalidBackgroundColorValue =
            std::format(L"[{}] is not a valid color value. Using default value {}.",
                        WIDEN(CFG_KEY_BACKGROUND_COLOR), WIDEN(CFG_BACKGROUND_COLOR_DEFAULT));

        // actions
        Shutdown = EN_SHUTDOWN;
        Sleep = EN_SLEEP;
        Restart = EN_RESTART;
        Lock = EN_LOCK;
        // English: first line like "Time until shutdown"
        Waiting[0] = L"Time until ";
        Waiting[1] = L"";
        Waiting[2] = L"";  // left empty
    }
}

std::wstring I18N::GetConfigWarningText(const std::vector<ConfigWarnInfo>& entries) const {
    std::wstring result = this->InvalidConfig + L"\n";
    for (const auto& entry : entries) {
        switch (entry.warning) {
            case ConfigWarning::InvalidLanguage:
                result += this->InvalidLanguage;
                break;
            case ConfigWarning::InvalidAction:
                result += this->InvalidAction;
                break;
            case ConfigWarning::InvalidInstruction:
                result += this->InvalidInstruction;
                break;
            case ConfigWarning::UnknownConfigKey:
                result += this->UnknownConfigKey;
                break;
            case ConfigWarning::NotConfigEntry:
                result += this->NotConfigEntry;
                break;
            case ConfigWarning::InvalidDelay:
                result += this->InvalidDelay;
                break;
            case ConfigWarning::InvalidBackgroundColorFormat:
                result += this->InvalidBackgroundColorFormat;
                break;
            case ConfigWarning::InvalidBackgroundColorValue:
                result += this->InvalidBackgroundColorValue;
                break;
            default:
                break;
        }
        result += std::format(L" ({} {})\n", this->LineNumber, entry.lineNo);
    }
    return result;
}

std::wstring I18N::Wait(Action type, int seconds) const {
    std::wstring actionWStr;
    switch (type) {
        case Action::Sleep:
            actionWStr = this->Sleep;
            break;
        case Action::Shutdown:
            actionWStr = this->Shutdown;
            break;
        case Action::Restart:
            actionWStr = this->Restart;
            break;
        case Action::Lock:
            actionWStr = this->Lock;
            break;
        case Action::None:
        default:
            actionWStr = L"Unknown Action";
            break;
    }
    // Return only the first-line text; seconds are drawn separately as a large centered number.
    return this->Waiting[0] + actionWStr + this->Waiting[1];
}

Action I18N::FileNameToAction(const std::wstring& name) const {
    std::wstring lowerName = name;
    for (auto& ch : lowerName) {
        ch = towlower(ch);
    }

    if (lowerName == EN_SLEEP || lowerName == ZH_SLEEP) {
        return Action::Sleep;
    }

    if (lowerName == EN_RESTART || lowerName == ZH_RESTART) {
        return Action::Restart;
    }

    if (lowerName == EN_SHUTDOWN || lowerName == ZH_SHUTDOWN) {
        return Action::Shutdown;
    }

    if (lowerName == EN_LOCK || lowerName == ZH_LOCK) {
        return Action::Lock;
    }

    return Action::None;
}
