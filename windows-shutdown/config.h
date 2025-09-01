#pragma once
#include "consts.h"	

struct Config {
  Lang lang = Lang::ZH;
  Mode mode = Mode::NORMAL;
  int delay = CFG_DEFAULT_DELAY;

  void Load();
  bool isImmediate() const { return mode == Mode::IMMEDIATE; }
};

