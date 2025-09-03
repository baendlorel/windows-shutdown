#pragma once
#include <string>
#include "consts.h"

#define EN_SHUTDOWN L"shutdown"
#define EN_SLEEP L"sleep"
#define EN_RESTART L"restart"
#define EN_LOCK L"lock"

#define ZH_SHUTDOWN L"关机"
#define ZH_SLEEP L"睡眠"
#define ZH_RESTART L"重启"
#define ZH_LOCK L"锁定"

class I18N {
   public:
    I18N(const I18N&) = delete;
    I18N& operator=(const I18N&) = delete;

    I18N(I18N&&) = delete;
    I18N& operator=(I18N&&) = delete;

    static I18N& GetInstance();
    void SetLang(Lang lang);

    // font family
    std::wstring FontFamilyName;

    // text members
    std::wstring ErrCreateWindow;
    std::wstring ErrCreateBitmap;
    std::wstring ErrResourceNotFound;
    std::wstring ErrLoadResource;
    std::wstring ErrCreateImageStream;
    std::wstring ErrCreateImageBitmap;
    std::wstring ErrGetProcessTokenRestart;
    std::wstring ErrGetProcessTokenShutdown;
    std::wstring ErrLookupPrivilegeRestart;
    std::wstring ErrLookupPrivilegeShutdown;
    std::wstring ErrRestartFailed;
    std::wstring ErrShutdownFailed;
    std::wstring ErrSleepFailed;
    std::wstring ErrTitle;
    std::wstring PressAnyKeyToExit;
    std::wstring PressAnyKeyToCancel;

    // actions
    std::wstring Shutdown;
    std::wstring Restart;
    std::wstring Sleep;
    std::wstring Waiting[3] = {L"", L"", L""};

    std::wstring Wait(Action type, int seconds) const;
    Action FileNameToAction(const std::wstring& name) const;

   private:
    I18N() = default;
};