#pragma once
#include <string>
#include <vector>
// order below must be preserved
#include <objidl.h>
#include <gdiplus.h>
#include "consts.h"

struct Config {
   public:
    Lang lang;
    Action action;
    Instruction instruction;
    int delay;
    Gdiplus::Color bgColor;  // ARGB , same as Gdiplus::Color
    std::vector<ConfigWarning> warnings;

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
