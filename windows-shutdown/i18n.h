#pragma once
#include <string>

#include "app-state.h"
#include "consts.h"

struct I18N {
   public:
    static void Init(const AppState& appState) {
        I18N::GetInstance().lang = appState.config.lang;
    }

    static I18N& GetInstance() {
        static I18N instance;
        return instance;
    }

   private:
    I18N();

    Lang lang = Lang::ZH;

   public:
    I18N(const I18N&) = delete;
    I18N& operator=(const I18N&) = delete;

    I18N(I18N&&) = delete;
    I18N& operator=(I18N&&) = delete;

    const wchar_t* FontFamilyName() {
        return lang == Lang::ZH ? L"Microsoft YaHei" : L"Arial";
    };

    std::wstring PressAnyKeyToExit() const;
    std::wstring PressAnyKeyToCancel() const;
    std::wstring Wait(Action type, int seconds) const;
};