#pragma once
#include <string>
// order below must be preserved
#include <gdiplus.h>
#include "consts.h"

struct Config {
   public:
    Lang lang;
    Action action;
    Instruction instruction;
    int delay;
    Gdiplus::Color bgColor;

   public:
    Config();

    bool isImmediate() const {
        return action != Action::None;
    }

    std::wstring GetConfigPath();

   private:
    void Load();
    std::string LoadKeyValue(std::string& key, std::string& value);
};
