#pragma once
#include <string>
#include <vector>
// order below must be preserved
#include <objidl.h>
#include <gdiplus.h>

// consts
#include "consts/configure.h"
#include "consts/core.h"

struct Config {
   public:
    Lang lang;
    Action action;
    Instruction instruction;
    int delay;
    Gdiplus::Color backgroundColor;  // ARGB , same as Gdiplus::Color
    std::vector<ConfigWarnInfo> warnings;

   public:
    Config();

    bool isImmediate() const {
        return action != Action::None;
    }

    std::wstring GetConfigPath();

   private:
    ConfigWarning LoadKeyValue(std::string& key, std::string& value);
    void Load();
};
