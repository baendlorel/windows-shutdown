#pragma once
#include <xstring>
#include "consts.h"

std::wstring GetConfigPath();

struct Config {
   public:
    Lang lang;
    Mode mode;
    int delay;

    // todo ������ʾinstruction��ѡ��

    Config();
    bool isImmediate() const {
        return mode == Mode::Immediate;
    }

   private:
    void Load();
};
