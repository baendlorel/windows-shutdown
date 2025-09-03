#pragma once
#include <xstring>
#include "consts.h"

struct Config {
   public:
    Lang lang;
    Mode mode;
    int delay;
    // todo 增加显示instruction的选项
   public:
    Config();
    bool isImmediate() const {
        return mode == Mode::Immediate;
    }
    std::wstring GetConfigPath();

   private:
    void Load();
};
