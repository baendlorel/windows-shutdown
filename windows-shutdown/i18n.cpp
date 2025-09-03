#include "i18n.h"

I18N& I18N::GetInstance() {
    static I18N instance;
    return instance;
}

void I18N::SetLang(const Lang lang) {
    if (lang == Lang::En) {
        I18N::GetInstance().dictionary = {
            {I18NKey::ErrorCreateWindow,
             L"Failed to create main window! The program cannot start."},
            {I18NKey::ErrorCreateBitmap,
             L"Failed to create bitmap, cannot display main interface!"},
            {I18NKey::ErrorResourceNotFound,
             L"Icon resource file not found! The program may be corrupted."},
            {I18NKey::ErrorLoadResource,
             L"Failed to load icon resource! The program may be corrupted."},
            {I18NKey::ErrorCreateImageStream, L"Failed to create image stream!"},
            {I18NKey::ErrorCreateImageBitmap, L"Failed to create icon bitmap!"},
            {I18NKey::ErrorGetProcessTokenRestart,
             L"Failed to get process privileges, cannot restart system!"},
            {I18NKey::ErrorGetProcessTokenShutdown,
             L"Failed to get process privileges, cannot shutdown system!"},
            {I18NKey::ErrorLookupPrivilegeRestart,
             L"Failed to lookup shutdown privilege, cannot restart system!"},
            {I18NKey::ErrorLookupPrivilegeShutdown,
             L"Failed to lookup shutdown privilege, cannot shutdown system!"},
            {I18NKey::ErrorRestartFailed, L"Failed to initiate system restart!"},
            {I18NKey::ErrorShutdownFailed, L"Failed to initiate system shutdown!"},
            {I18NKey::ErrorSleepFailed,
             L"System sleep failed! Sleep function may not be supported."},
            {I18NKey::ErrorTitle, L"Error"},
            {I18NKey::PressAnyKeyToExit, L"Press any key or click elsewhere to exit"},
            {I18NKey::PressAnyKeyToCancel, L"Press any key or click to cancel"}};
    } else {
        I18N::GetInstance().dictionary = {
            {I18NKey::ErrorCreateWindow, L"创建主窗口失败！程序无法启动。"},
            {I18NKey::ErrorCreateBitmap, L"创建位图失败，无法显示主界面！"},
            {I18NKey::ErrorResourceNotFound, L"找不到图标资源文件！程序可能损坏。"},
            {I18NKey::ErrorLoadResource, L"加载图标资源失败！程序可能损坏。"},
            {I18NKey::ErrorCreateImageStream, L"创建图像流失败！"},
            {I18NKey::ErrorCreateImageBitmap, L"创建图标位图失败！"},
            {I18NKey::ErrorGetProcessTokenRestart, L"获取进程权限失败，无法重启系统！"},
            {I18NKey::ErrorGetProcessTokenShutdown, L"获取进程权限失败，无法关闭系统！"},
            {I18NKey::ErrorLookupPrivilegeRestart, L"查找关机权限失败，无法重启系统！"},
            {I18NKey::ErrorLookupPrivilegeShutdown, L"查找关机权限失败，无法关闭系统！"},
            {I18NKey::ErrorRestartFailed, L"启动系统重启失败！"},
            {I18NKey::ErrorShutdownFailed, L"启动系统关机失败！"},
            {I18NKey::ErrorSleepFailed, L"系统睡眠失败！可能不支持睡眠功能。"},
            {I18NKey::ErrorTitle, L"错误"},
            {I18NKey::PressAnyKeyToExit, L"按任意键或鼠标点击其他位置退出"},
            {I18NKey::PressAnyKeyToCancel, L"按任意键或鼠标点击取消"}};
    }
}

std::wstring I18N::Get(I18NKey key) const {
    auto it = this->dictionary.find(key);
    if (it != this->dictionary.end()) {
        return it->second;
    }
    return L"Unknown Error";
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
    // auto& appState = AppState::GetInstance();
    // std::wstring exePath = GetConfigPath();
    // size_t pos1 = exePath.find_last_of(L"\\/");
    // std::wstring name = (pos1 != std::wstring::npos) ? exePath.substr(pos1 + 1) : exePath;
    // size_t pos2 = name.find_last_of(L".");
    // if (pos2 != std::wstring::npos) {
    //     name = name.substr(0, pos2);
    // }
    //  To lower case
    // for (auto& ch : name) {
    //    ch = towlower(ch);
    //}

    if (name == L"sleep" || name == L"睡眠") {
        return Action::Sleep;
    } else if (name == L"restart" || name == L"重启") {
        return Action::Restart;
    } else if (name == L"shutdown" || name == L"关机") {
        return Action::Shutdown;
    }
    return Action::None;
}

std::wstring GetText(I18NKey key) {
    static auto& i18n = I18N::GetInstance();
    return i18n.Get(key);
}

const wchar_t* I18NWChar(I18NKey key) {
    static auto& i18n = I18N::GetInstance();
    return i18n.Get(key).c_str();
}
