#pragma once

enum class Mode { IMMEDIATE, NORMAL };
constexpr int DEFAULT_DELAY = 5;

struct Config {
  Mode mode = Mode::NORMAL;
  int delay = DEFAULT_DELAY;

  void Load();
};
