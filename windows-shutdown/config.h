#pragma once
#include <xstring>
#include "consts.h"

struct Config {
   public:
    Lang lang;
    Mode mode;
    int delay;
    // todo ������ʾinstruction��ѡ��
   public:
    Config();
    bool isImmediate() const {
        return mode == Mode::Immediate;
    }
    std::wstring GetConfigPath();

   private:
    void Load();
};
