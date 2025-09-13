#pragma once
#include <xstring>
#include <format>

std::wstring FormatTime(int seconds) {
	static auto pad = [](int num) {
		return (num > 9) ? std::format(L"{}", num) : std::format(L"0{}", num);
		};

	int h = seconds / 3600;
	int m = (seconds % 3600) / 60;
	int s = seconds % 60;
	return std::format(L"{}:{}:{}", pad(h), pad(m), pad(s));
}