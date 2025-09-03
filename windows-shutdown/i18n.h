#pragma once
#include <string>
#include <unordered_map>
#include "consts.h"

enum class I18NKey {
    ErrorCreateWindow,
    ErrorCreateBitmap,
    ErrorResourceNotFound,
    ErrorLoadResource,
    ErrorCreateImageStream,
    ErrorCreateImageBitmap,
    ErrorGetProcessTokenRestart,
    ErrorGetProcessTokenShutdown,
    ErrorLookupPrivilegeRestart,
    ErrorLookupPrivilegeShutdown,
    ErrorRestartFailed,
    ErrorShutdownFailed,
    ErrorSleepFailed,
    ErrorTitle,
    PressAnyKeyToExit,
    PressAnyKeyToCancel
};

class I18N {
   public:
    static I18N& GetInstance();

    void SetLang(Lang l);

    std::wstring Get(I18NKey key) const;

    std::wstring Wait(Action type, int seconds) const;

    Action FileNameToAction(const std::wstring& name) const;

   private:
    I18N();
    Lang lang;
    std::unordered_map<I18NKey, std::wstring> zh;
    std::unordered_map<I18NKey, std::wstring> en;
};