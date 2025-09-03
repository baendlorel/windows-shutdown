#include "i18n.h"
#include "app-state.h"
#include "config.h"

I18N& I18N::GetInstance() {
    static I18N instance;
    return instance;
}

void I18N::SetLang(Lang lang) {
    if (lang == Lang::Zh) {
        ErrCreateWindow = L"����������ʧ�ܣ������޷�������";
        ErrCreateBitmap = L"����λͼʧ�ܣ��޷���ʾ�����棡";
        ErrResourceNotFound = L"�Ҳ���ͼ����Դ�ļ�����������𻵡�";
        ErrLoadResource = L"����ͼ����Դʧ�ܣ���������𻵡�";
        ErrCreateImageStream = L"����ͼ����ʧ�ܣ�";
        ErrCreateImageBitmap = L"����ͼ��λͼʧ�ܣ�";
        ErrGetProcessTokenRestart = L"��ȡ����Ȩ��ʧ�ܣ��޷�����ϵͳ��";
        ErrGetProcessTokenShutdown = L"��ȡ����Ȩ��ʧ�ܣ��޷��ر�ϵͳ��";
        ErrLookupPrivilegeRestart = L"���ҹػ�Ȩ��ʧ�ܣ��޷�����ϵͳ��";
        ErrLookupPrivilegeShutdown = L"���ҹػ�Ȩ��ʧ�ܣ��޷��ر�ϵͳ��";
        ErrRestartFailed = L"����ϵͳ����ʧ�ܣ�";
        ErrShutdownFailed = L"����ϵͳ�ػ�ʧ�ܣ�";
        ErrSleepFailed = L"ϵͳ˯��ʧ�ܣ����ܲ�֧��˯�߹��ܡ�";
        ErrTitle = L"����";
        PressAnyKeyToExit = L"������������������λ���˳�";
        PressAnyKeyToCancel = L"��������������ȡ��";
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
    std::wstring lowerName = name;
    for (auto& ch : lowerName) ch = towlower(ch);
    if (lowerName == L"sleep" || lowerName == L"˯��") {
        return Action::Sleep;
    } else if (lowerName == L"restart" || lowerName == L"����") {
        return Action::Restart;
    } else if (lowerName == L"shutdown" || lowerName == L"�ػ�") {
        return Action::Shutdown;
    }
    return Action::None;
}
