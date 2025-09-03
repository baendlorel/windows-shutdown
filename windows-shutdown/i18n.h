#pragma once
#include <string>
#include "consts.h"

class I18N {
   public:
    I18N(const I18N&) = delete;
    I18N& operator=(const I18N&) = delete;

    I18N(I18N&&) = delete;
    I18N& operator=(I18N&&) = delete;

    static I18N& GetInstance();
    void SetLang(Lang lang);

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

    std::wstring Wait(Action type, int seconds) const;
    Action FileNameToAction(const std::wstring& name) const;

   private:
    I18N() = default;
};