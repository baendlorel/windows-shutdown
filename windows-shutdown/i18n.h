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
    static I18N& GetInstance(const Lang lang = Lang::Default);

    static void SetLang(const Lang lang);

    std::wstring Get(I18NKey key) const;

    std::wstring Wait(Action type, int seconds) const;

    Action FileNameToAction(const std::wstring& name) const;

   private:
    I18N() : dictionary({}) {};
    std::unordered_map<I18NKey, std::wstring> dictionary;
};