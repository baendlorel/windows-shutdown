#pragma once

#define CFG_KEY_LANG "lang"
#define CFG_KEY_MODE "mode"
#define CFG_KEY_DELAY "delay"

#define CFG_LANG_ZH "zh"
#define CFG_LANG_EN "en"

#define CFG_MODE_IMMEDIATE "immediate"
#define CFG_MODE_NORMAL "normal"

enum Mode { IMMEDIATE, NORMAL };
constexpr int CFG_DEFAULT_DELAY = 3;

struct Config {
  bool isZh = true;
  bool isImmediate = false;
  int delay = CFG_DEFAULT_DELAY;

  void Load();
};

