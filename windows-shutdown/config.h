#pragma once
#include <xstring>
#include "consts.h"

std::wstring GetConfigPath();

struct Config {
   public:
    Lang lang;
    Mode mode;
    int delay;

    Config();
    bool isImmediate() const { return mode == Mode::IMMEDIATE; }

   private:
    void Load();
};
