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

    static void SetLang(const Lang lang);

    std::wstring Get(I18NKey key) const;

    std::wstring Wait(Action type, int seconds) const;

    Action FileNameToAction(const std::wstring& name) const;

   public:
    std::wstring ErrorTitle;

   private:
    I18N() : dictionary({}) {};
    std::unordered_map<I18NKey, std::wstring> dictionary;
};

std::wstring GetText(I18NKey key);
const wchar_t* I18NWChar(I18NKey key);