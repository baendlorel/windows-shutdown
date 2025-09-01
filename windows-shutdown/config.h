#pragma once
#include "consts.h"	

struct Config {
  Lang lang = Lang::ZH;
  bool isImmediate = false;
  int delay = CFG_DEFAULT_DELAY;

  void Load();
};

