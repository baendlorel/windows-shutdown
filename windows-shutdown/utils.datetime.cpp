#include "utils.datetime.h"
#include <format>

std::wstring format_time(const int seconds) {
    static auto pad = [](int num) {
        return num > 9 ? std::format(L"{}", num) : std::format(L"0{}", num);
    };

    const int h = seconds / 3600;
    const int min_sec = seconds % 3600;
    const int m = min_sec / 60;
    const int s = min_sec % 60;
    return std::format(L"{}:{}:{}", pad(h), pad(m), pad(s));
}
