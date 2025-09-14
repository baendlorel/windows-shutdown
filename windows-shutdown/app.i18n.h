#pragma once
#include <string>
#include <vector>
#include "singleton.h"
#include "consts.config.h"
#include "consts.app.h"

#define EN_SHUTDOWN L"shutdown"
#define EN_SLEEP    L"sleep"
#define EN_RESTART  L"restart"
#define EN_LOCK     L"lock"

#define ZH_SHUTDOWN L"关机"
#define ZH_SLEEP    L"睡眠"
#define ZH_RESTART  L"重启"
#define ZH_LOCK     L"锁定"

class AppI18N {
    SINGLETON(AppI18N)

   public:
    void SetLang(CFG::Lang lang);

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

    // meta info
    std::wstring LineNumber;

    // config warning messages
    std::wstring InvalidConfig;
    std::wstring InvalidLanguage;
    std::wstring UnknownConfigKey;
    std::wstring NotConfigEntry;
    std::wstring InvalidAction;
    std::wstring InvalidInstruction;
    std::wstring InvalidMenuButton;
    std::wstring InvalidCountdownStyle;
    std::wstring InvalidDelay;
    std::wstring InvalidBackgroundColorFormat;
    std::wstring InvalidBackgroundColorValue;

    std::wstring GetConfigWarningText(const std::vector<CFG::WarnInfo>& warnings) const;

    // actions
    std::wstring Shutdown;
    std::wstring Restart;
    std::wstring Sleep;
    std::wstring Lock;
    std::wstring Waiting[3] = {L"", L"", L""};

    std::wstring Wait(Action type, int seconds) const;

    // zh and en uses different resource ids
    int DonateQRResourceID = 0;
    std::wstring Author = L"Kasukabe Tsumugi<futami16237@gmail.com>";
    std::wstring AuthorInfo;
    std::wstring DonateThank;
};
