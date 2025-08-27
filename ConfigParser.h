#pragma once
#include <string>

struct WindowsShutdownConfig {
    std::string mode;
};

WindowsShutdownConfig ParseConfigFile(const std::wstring& path);
