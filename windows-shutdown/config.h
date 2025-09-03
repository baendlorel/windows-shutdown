#pragma once
#include <string>
#include "consts.h"

struct Config {
   public:
    Lang lang;
    Action action;
    Instruction instruction;
    int delay;

   public:
    Config();

    bool isImmediate() const {
        return action != Action::None;
    }

    std::wstring GetConfigPath();

   private:
    void Load();
};
