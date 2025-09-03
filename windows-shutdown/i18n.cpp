#include "i18n.h"

I18N::I18N() = default;

std::wstring I18N::PressAnyKeyToExit() const {
    if (lang == Lang::EN) {
        return L"Press any key or click elsewhere to exit";
    } else {
        return L"������������������λ���˳�";
    }
}

std::wstring I18N::PressAnyKeyToCancel() const {
    if (lang == Lang::EN) {
        return L"Press any key or click to cancel";
    } else {
        return L"��������������ȡ��";
    }
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
    if (lang == Lang::EN) {
        return L"About to " + action_en + L", " + std::to_wstring(seconds) + L" seconds left...";
    } else {
        return L"����" + action_zh + L"��ʣ��" + std::to_wstring(seconds) + L"��...";
    }
}

Action I18N::FileNameToAction() const {
    auto& appState = AppState::GetInstance();
    std::wstring exePath = GetConfigPath();
    size_t pos1 = exePath.find_last_of(L"\\/");
    std::wstring name = (pos1 != std::wstring::npos) ? exePath.substr(pos1 + 1) : exePath;
    size_t pos2 = name.find_last_of(L".");
    if (pos2 != std::wstring::npos) {
        name = name.substr(0, pos2);
    }
    // To lower case
    for (auto& ch : name) ch = towlower(ch);

    if (name == L"sleep" || name == L"˯��") {
        return Action::Sleep;
    } else if (name == L"restart" || name == L"����") {
        return Action::Restart;
    } else if (name == L"shutdown" || name == L"�ػ�") {
        return Action::Shutdown;
    }
    return Action::None;
}

// Error messages
std::wstring I18N::ErrorCreateWindow() const {
    if (lang == Lang::EN) {
        return L"Failed to create main window! The program cannot start.";
    } else {
        return L"����������ʧ�ܣ������޷�������";
    }
}

std::wstring I18N::ErrorCreateBitmap() const {
    if (lang == Lang::EN) {
        return L"Failed to create bitmap, cannot display main interface!";
    } else {
        return L"����λͼʧ�ܣ��޷���ʾ�����棡";
    }
}

std::wstring I18N::ErrorResourceNotFound() const {
    if (lang == Lang::EN) {
        return L"Icon resource file not found! The program may be corrupted.";
    } else {
        return L"�Ҳ���ͼ����Դ�ļ�����������𻵡�";
    }
}

std::wstring I18N::ErrorLoadResource() const {
    if (lang == Lang::EN) {
        return L"Failed to load icon resource! The program may be corrupted.";
    } else {
        return L"����ͼ����Դʧ�ܣ���������𻵡�";
    }
}

std::wstring I18N::ErrorCreateImageStream() const {
    if (lang == Lang::EN) {
        return L"Failed to create image stream!";
    } else {
        return L"����ͼ����ʧ�ܣ�";
    }
}

std::wstring I18N::ErrorCreateImageBitmap() const {
    if (lang == Lang::EN) {
        return L"Failed to create icon bitmap!";
    } else {
        return L"����ͼ��λͼʧ�ܣ�";
    }
}

std::wstring I18N::ErrorGetProcessTokenRestart() const {
    if (lang == Lang::EN) {
        return L"Failed to get process privileges, cannot restart system!";
    } else {
        return L"��ȡ����Ȩ��ʧ�ܣ��޷�����ϵͳ��";
    }
}

std::wstring I18N::ErrorGetProcessTokenShutdown() const {
    if (lang == Lang::EN) {
        return L"Failed to get process privileges, cannot shutdown system!";
    } else {
        return L"��ȡ����Ȩ��ʧ�ܣ��޷��ر�ϵͳ��";
    }
}

std::wstring I18N::ErrorLookupPrivilegeRestart() const {
    if (lang == Lang::EN) {
        return L"Failed to lookup shutdown privilege, cannot restart system!";
    } else {
        return L"���ҹػ�Ȩ��ʧ�ܣ��޷�����ϵͳ��";
    }
}

std::wstring I18N::ErrorLookupPrivilegeShutdown() const {
    if (lang == Lang::EN) {
        return L"Failed to lookup shutdown privilege, cannot shutdown system!";
    } else {
        return L"���ҹػ�Ȩ��ʧ�ܣ��޷��ر�ϵͳ��";
    }
}

std::wstring I18N::ErrorRestartFailed() const {
    if (lang == Lang::EN) {
        return L"Failed to initiate system restart!";
    } else {
        return L"����ϵͳ����ʧ�ܣ�";
    }
}

std::wstring I18N::ErrorShutdownFailed() const {
    if (lang == Lang::EN) {
        return L"Failed to initiate system shutdown!";
    } else {
        return L"����ϵͳ�ػ�ʧ�ܣ�";
    }
}

std::wstring I18N::ErrorSleepFailed() const {
    if (lang == Lang::EN) {
        return L"System sleep failed! Sleep function may not be supported.";
    } else {
        return L"ϵͳ˯��ʧ�ܣ����ܲ�֧��˯�߹��ܡ�";
    }
}

std::wstring I18N::ErrorTitle() const {
    if (lang == Lang::EN) {
        return L"Error";
    } else {
        return L"����";
    }
}
