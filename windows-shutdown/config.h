#pragma once
#include <xstring>
#include "consts.h"

struct Config {
   public:
    Lang lang;
    Mode mode;
    Instruction instruction;
    int delay;

   public:
    Config();

    bool isImmediate() const {
        return mode != Mode::Normal;
    }

    std::wstring GetConfigPath();

   private:
    void Load();
};
