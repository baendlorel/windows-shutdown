#include "i18n.h"
#include "app-state.h"
#include "config.h"

I18N& I18N::GetInstance() {
    static I18N instance;
    return instance;
}

void I18N::SetLang(Lang lang) {
    if (lang == Lang::Zh) {
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
        PressAnyKeyToExit = L"按任意键或鼠标点击其他位置退出";
        PressAnyKeyToCancel = L"按任意键或鼠标点击取消";
    } else {
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
        PressAnyKeyToExit = L"Press any key or click elsewhere to exit";
        PressAnyKeyToCancel = L"Press any key or click to cancel";
    }
}

std::wstring I18N::Wait(Action type, int seconds) const {
    std::wstring action_zh, action_en;
    switch (type) {
        case Action::Sleep:
            action_zh = L"睡眠";
            action_en = L"sleep";
            break;
        case Action::Shutdown:
            action_zh = L"关机";
            action_en = L"shutdown";
            break;
        case Action::Restart:
            action_zh = L"重启";
            action_en = L"restart";
            break;
    }
    if (lang == Lang::En) {
        return L"About to " + action_en + L", " + std::to_wstring(seconds) + L" seconds left...";
    } else {
        return L"即将" + action_zh + L"，剩余" + std::to_wstring(seconds) + L"秒...";
    }
}

Action I18N::FileNameToAction(const std::wstring& name) const {
    std::wstring lowerName = name;
    for (auto& ch : lowerName) ch = towlower(ch);
    if (lowerName == L"sleep" || lowerName == L"睡眠") {
        return Action::Sleep;
    } else if (lowerName == L"restart" || lowerName == L"重启") {
        return Action::Restart;
    } else if (lowerName == L"shutdown" || lowerName == L"关机") {
        return Action::Shutdown;
    }
    return Action::None;
}
