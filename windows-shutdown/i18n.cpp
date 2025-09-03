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
            {I18NKey::ErrorCreateWindow, L"����������ʧ�ܣ������޷�������"},
            {I18NKey::ErrorCreateBitmap, L"����λͼʧ�ܣ��޷���ʾ�����棡"},
            {I18NKey::ErrorResourceNotFound, L"�Ҳ���ͼ����Դ�ļ�����������𻵡�"},
            {I18NKey::ErrorLoadResource, L"����ͼ����Դʧ�ܣ���������𻵡�"},
            {I18NKey::ErrorCreateImageStream, L"����ͼ����ʧ�ܣ�"},
            {I18NKey::ErrorCreateImageBitmap, L"����ͼ��λͼʧ�ܣ�"},
            {I18NKey::ErrorGetProcessTokenRestart, L"��ȡ����Ȩ��ʧ�ܣ��޷�����ϵͳ��"},
            {I18NKey::ErrorGetProcessTokenShutdown, L"��ȡ����Ȩ��ʧ�ܣ��޷��ر�ϵͳ��"},
            {I18NKey::ErrorLookupPrivilegeRestart, L"���ҹػ�Ȩ��ʧ�ܣ��޷�����ϵͳ��"},
            {I18NKey::ErrorLookupPrivilegeShutdown, L"���ҹػ�Ȩ��ʧ�ܣ��޷��ر�ϵͳ��"},
            {I18NKey::ErrorRestartFailed, L"����ϵͳ����ʧ�ܣ�"},
            {I18NKey::ErrorShutdownFailed, L"����ϵͳ�ػ�ʧ�ܣ�"},
            {I18NKey::ErrorSleepFailed, L"ϵͳ˯��ʧ�ܣ����ܲ�֧��˯�߹��ܡ�"},
            {I18NKey::ErrorTitle, L"����"},
            {I18NKey::PressAnyKeyToExit, L"������������������λ���˳�"},
            {I18NKey::PressAnyKeyToCancel, L"��������������ȡ��"}};
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
            action_zh = L"˯��";
            action_en = L"sleep";
            break;
        case Action::Shutdown:
            action_zh = L"�ػ�";
            action_en = L"shutdown";
            break;
        case Action::Restart:
            action_zh = L"����";
            action_en = L"restart";
            break;
    }
    if (lang == Lang::En) {
        return L"About to " + action_en + L", " + std::to_wstring(seconds) + L" seconds left...";
    } else {
        return L"����" + action_zh + L"��ʣ��" + std::to_wstring(seconds) + L"��...";
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

    if (name == L"sleep" || name == L"˯��") {
        return Action::Sleep;
    } else if (name == L"restart" || name == L"����") {
        return Action::Restart;
    } else if (name == L"shutdown" || name == L"�ػ�") {
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
