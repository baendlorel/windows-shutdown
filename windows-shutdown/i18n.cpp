#include "i18n.h"

I18N& I18N::GetInstance() {
    static I18N instance;
    return instance;
}

void I18N::SetLang(Lang lang) {
    if (lang == Lang::Zh) {
        FontFamilyName = L"Microsoft YaHei UI";

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

        // actions
        Shutdown = ZH_SHUTDOWN;
        Sleep = ZH_SLEEP;
        Restart = ZH_RESTART;
        Waiting[0] = L"����";
        Waiting[1] = L"��ʣ��";
        Waiting[2] = L"��...";
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
        PressAnyKeyToExit = L"Press any key or click elsewhere to exit";
        PressAnyKeyToCancel = L"Press any key or click to cancel";

        // actions
        Shutdown = EN_SHUTDOWN;
        Sleep = EN_SLEEP;
        Restart = EN_RESTART;
        Waiting[0] = L"About to";
        Waiting[1] = L", ";
        Waiting[2] = L" seconds left...";
    }
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
    }
    return this->Waiting[0] + actionWStr + this->Waiting[1] + std::to_wstring(seconds) +
           this->Waiting[2];
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
