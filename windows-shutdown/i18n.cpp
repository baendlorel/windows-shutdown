#include "i18n.h"

I18N::I18N() = default;

std::wstring I18N::PressAnyKeyToExit() const {
    if (lang == Lang::EN) {
        return L"Press any key or click elsewhere to exit";
    } else {
        return L"按任意键或鼠标点击其他位置退出";
    }
}

std::wstring I18N::PressAnyKeyToCancel() const {
    if (lang == Lang::EN) {
        return L"Press any key or click to cancel";
    } else {
        return L"按任意键或鼠标点击取消";
    }
}

std::wstring I18N::Wait(Action type, int seconds) const {
    std::wstring action_zh, action_en;
    switch (type) {
        case Action::Sleep:
            action_zh = L"睡眠";
            action_en = L"sleep";
            break;
        case Action::Shutdown:
            action_zh = L"关机";
            action_en = L"shutdown";
            break;
        case Action::Restart:
            action_zh = L"重启";
            action_en = L"restart";
            break;
    }
    if (lang == Lang::EN) {
        return L"About to " + action_en + L", " + std::to_wstring(seconds) + L" seconds left...";
    } else {
        return L"即将" + action_zh + L"，剩余" + std::to_wstring(seconds) + L"秒...";
    }
}
