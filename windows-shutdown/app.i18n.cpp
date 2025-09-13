#include "app.i18n.h"
#include <format>
#include "resource.h"
#include "widen.h"

void AppI18N::SetLang(Lang lang) {
    if (lang == Lang::Zh) {
        this->FontFamilyName = L"Microsoft YaHei UI";

        this->ErrCreateWindow = L"创建主窗口失败！程序无法启动。";
        this->ErrCreateBitmap = L"创建位图失败，无法显示主界面！";
        this->ErrResourceNotFound = L"找不到图标资源文件！程序可能损坏。";
        this->ErrLoadResource = L"加载图标资源失败！程序可能损坏。";
        this->ErrCreateImageStream = L"创建图像流失败！";
        this->ErrCreateImageBitmap = L"创建图标位图失败！";
        this->ErrGetProcessTokenRestart = L"获取进程权限失败，无法重启系统！";
        this->ErrGetProcessTokenShutdown = L"获取进程权限失败，无法关闭系统！";
        this->ErrLookupPrivilegeRestart = L"查找关机权限失败，无法重启系统！";
        this->ErrLookupPrivilegeShutdown = L"查找关机权限失败，无法关闭系统！";
        this->ErrRestartFailed = L"启动系统重启失败！";
        this->ErrShutdownFailed = L"启动系统关机失败！";
        this->ErrSleepFailed = L"系统睡眠失败！可能不支持睡眠功能。";
        this->ErrTitle = L"错误";
        this->PressAnyKeyToExit = L"按 ESC 键或鼠标点击其他位置退出。";
        this->PressAnyKeyToCancel = L"按 ESC 键或鼠标点击取消。";

        this->LineNumber = L"行号";

        // config warnings
        this->InvalidConfig = L"config.txt 配置文件里有无效配置：";
        this->UnknownConfigKey = std::format(L"未知的配置项，已忽略。");
        this->NotConfigEntry = L"配置文件中有无法解析的行，已忽略。";
        this->InvalidLanguage = std::format(
            L"[{}]的值无效，应该是\"{},{}\"中的一个。现使用默认值{}。", WIDEN(CFG_KEY_LANG),
            WIDEN(CFG_LANG_ZH), WIDEN(CFG_LANG_EN), WIDEN(CFG_LANG_EN));
        this->InvalidAction =
            std::format(L"[{}]的值无效，应该是\"{}\"中的一个。现使用默认值{}。",
                        WIDEN(CFG_KEY_ACTION), WIDEN(CFG_ACTION_SOME), WIDEN(CFG_ACTION_NONE));
        this->InvalidInstruction = std::format(
            L"[{}]的值无效，应该是\"{}, {}\"中的一个。现使用默认值{}。", WIDEN(CFG_KEY_INSTRUCTION),
            WIDEN(CFG_INSTRUCTION_SHOW), WIDEN(CFG_INSTRUCTION_HIDE), WIDEN(CFG_INSTRUCTION_SHOW));
        this->InvalidMenuButton = std::format(
            L"[{}]的值无效，应该是以英文逗号分隔的Action枚举名称（如：{}）。现使用默认值。{}",
            WIDEN(CFG_KEY_MENU_BUTTONS), WIDEN(CFG_MENU_BUTTONS_SOME),
            WIDEN(CFG_MENU_BUTTONS_SOME));
        this->InvalidCountdownStyle = std::format(
            L"[{}]的值无效，应该是\"{}, {}\"中的一个。现使用默认值{}。",
            WIDEN(CFG_KEY_COUNTDOWN_STYLE), WIDEN(CFG_COUNTDOWN_STYLE_NORMAL),
            WIDEN(CFG_COUNTDOWN_STYLE_STEINS_GATE), WIDEN(CFG_COUNTDOWN_STYLE_STEINS_GATE));
        this->InvalidDelay = std::format(L"[{}]必须是大于或等于0的整数。现使用默认值{}。",
                                         WIDEN(CFG_KEY_DELAY), CFG_DEFAULT_DELAY);
        this->InvalidBackgroundColorFormat =
            std::format(L"[{}]不是有效的#RRGGBBAA或#RRGGBB格式。现使用默认值{}。",
                        WIDEN(CFG_KEY_BACKGROUND_COLOR), WIDEN(CFG_BACKGROUND_COLOR_DEFAULT));
        this->InvalidBackgroundColorValue =
            std::format(L"[{}]不是有效的颜色值。现使用默认值{}。", WIDEN(CFG_KEY_BACKGROUND_COLOR),
                        WIDEN(CFG_BACKGROUND_COLOR_DEFAULT));

        // actions
        this->Shutdown = ZH_SHUTDOWN;
        this->Sleep = ZH_SLEEP;
        this->Restart = ZH_RESTART;
        this->Lock = ZH_LOCK;
        this->Waiting[0] = L"距离";  // e.g. "距离关机还有"
        this->Waiting[1] = L"还有";
        this->Waiting[2] = L"";  // left empty

        this->DonateQRResourceID = IDB_DONATEZHPNG;
        this->AuthorInfo = L"欢迎访问 https://baendlorel.github.io/ 查看更多有趣项目和App";
        this->DonateThank = L"谢谢你的支持，给我满满的动力～";
    } else {
        this->FontFamilyName = L"Arial";

        this->ErrCreateWindow = L"Failed to create main window! The program cannot start.";
        this->ErrCreateBitmap = L"Failed to create bitmap, cannot display main interface!";
        this->ErrResourceNotFound = L"Icon resource file not found! The program may be corrupted.";
        this->ErrLoadResource = L"Failed to load icon resource! The program may be corrupted.";
        this->ErrCreateImageStream = L"Failed to create image stream!";
        this->ErrCreateImageBitmap = L"Failed to create icon bitmap!";
        this->ErrGetProcessTokenRestart =
            L"Failed to get process privileges, cannot restart system!";
        this->ErrGetProcessTokenShutdown =
            L"Failed to get process privileges, cannot shutdown system!";
        this->ErrLookupPrivilegeRestart =
            L"Failed to lookup shutdown privilege, cannot restart system!";
        this->ErrLookupPrivilegeShutdown =
            L"Failed to lookup shutdown privilege, cannot shutdown system!";
        this->ErrRestartFailed = L"Failed to initiate system restart!";
        this->ErrShutdownFailed = L"Failed to initiate system shutdown!";
        this->ErrSleepFailed = L"System sleep failed! Sleep function may not be supported.";
        this->ErrTitle = L"Error";
        this->PressAnyKeyToExit = L"Press ESC key or click elsewhere to exit.";
        this->PressAnyKeyToCancel = L"Press ESC key or click to cancel.";

        this->LineNumber = L"Line";

        // config warnings
        this->InvalidConfig = L"Configuration file has some invalid values:";
        this->UnknownConfigKey = L"Unknown configuration key ignored.";
        this->NotConfigEntry = L"Unrecognized line in configuration file ignored.";
        this->InvalidLanguage = std::format(
            L"[{}] is not valid, should be {} or {}. Using default value {}.", WIDEN(CFG_KEY_LANG),
            WIDEN(CFG_LANG_ZH), WIDEN(CFG_LANG_EN), WIDEN(CFG_LANG_EN));
        this->InvalidAction =
            std::format(L"[{}] is not valid, should be one of {}. Using default value {}.",
                        WIDEN(CFG_KEY_ACTION), WIDEN(CFG_ACTION_SOME), WIDEN(CFG_ACTION_NONE));
        this->InvalidInstruction =
            std::format(L"[{}] is not valid, should be {} or {}. Using default value {}.",
                        WIDEN(CFG_KEY_INSTRUCTION), WIDEN(CFG_INSTRUCTION_SHOW),
                        WIDEN(CFG_INSTRUCTION_HIDE), WIDEN(CFG_INSTRUCTION_SHOW));
        this->InvalidMenuButton = std::format(
            L"[{}] is not valid, should be comma-separated Action enum names (like: {}). Using "
            L"default value {}.",
            WIDEN(CFG_KEY_MENU_BUTTONS), WIDEN(CFG_MENU_BUTTONS_SOME),
            WIDEN(CFG_MENU_BUTTONS_SOME));
        this->InvalidCountdownStyle = std::format(
            L"[{}] is not valid, should be {} or {}. Using default value {}.",
            WIDEN(CFG_KEY_COUNTDOWN_STYLE), WIDEN(CFG_COUNTDOWN_STYLE_NORMAL),
            WIDEN(CFG_COUNTDOWN_STYLE_STEINS_GATE), WIDEN(CFG_COUNTDOWN_STYLE_STEINS_GATE));
        this->InvalidDelay =
            std::format(L"[{}] must be a non-negative interger. Using default value {}.",
                        WIDEN(CFG_KEY_DELAY), CFG_DEFAULT_DELAY);
        this->InvalidBackgroundColorFormat =
            std::format(L"[{}] is not valid #RRGGBBAA or #RRGGBB format. Using default value {}.",
                        WIDEN(CFG_KEY_BACKGROUND_COLOR), WIDEN(CFG_BACKGROUND_COLOR_DEFAULT));
        this->InvalidBackgroundColorValue =
            std::format(L"[{}] is not a valid color value. Using default value {}.",
                        WIDEN(CFG_KEY_BACKGROUND_COLOR), WIDEN(CFG_BACKGROUND_COLOR_DEFAULT));

        // actions
        this->Shutdown = EN_SHUTDOWN;
        this->Sleep = EN_SLEEP;
        this->Restart = EN_RESTART;
        this->Lock = EN_LOCK;
        // English: first line like "Time until shutdown"
        this->Waiting[0] = L"Time until ";
        this->Waiting[1] = L"";
        this->Waiting[2] = L"";  // left empty

        this->DonateQRResourceID = IDB_DONATEENPNG;
        this->AuthorInfo = L"Welcome to https://baendlorel.github.io/ for more interesting apps\n";

        this->DonateThank = L"Thanks for your support — you keep me going!";
    }
}

std::wstring AppI18N::GetConfigWarningText(const std::vector<ConfigWarnInfo>& entries) const {
    if (entries.empty()) {
        return std::wstring();
    }

    std::wstring result = this->InvalidConfig + L"\n";
    for (int i = 0; i < entries.size(); i++) {
        auto& entry = entries[i];
        std::wstring text;

        switch (entry.warning) {
            case ConfigWarning::InvalidLanguage:
                text = this->InvalidLanguage;
                break;
            case ConfigWarning::InvalidAction:
                text = this->InvalidAction;
                break;
            case ConfigWarning::InvalidInstruction:
                text = this->InvalidInstruction;
                break;
            case ConfigWarning::InvalidMenuButton:
                text = this->InvalidMenuButton;
                break;
            case ConfigWarning::InvalidCountdownStyle:
                text = this->InvalidCountdownStyle;
                break;
            case ConfigWarning::UnknownConfigKey:
                text = this->UnknownConfigKey;
                break;
            case ConfigWarning::NotConfigEntry:
                text = this->NotConfigEntry;
                break;
            case ConfigWarning::InvalidDelay:
                text = this->InvalidDelay;
                break;
            case ConfigWarning::InvalidBackgroundColorFormat:
                text = this->InvalidBackgroundColorFormat;
                break;
            case ConfigWarning::InvalidBackgroundColorValue:
                text = this->InvalidBackgroundColorValue;
                break;
            default:
                break;
        }
        result += std::format(L"    {}. {} ({} {})\n", i + 1, text, this->LineNumber, entry.lineNo);
    }

    return result;
}

std::wstring AppI18N::Wait(Action type, int seconds) const {
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
