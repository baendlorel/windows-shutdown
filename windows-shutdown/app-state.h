#pragma once
#include "Resource.h"
#include "consts.h"
#include "config.h"
#include "image-button.h"

class AppState {
 public:
  static AppState& getInstance() {
    static AppState instance;
    return instance;
  }

  AppState() {
    this->hInst = nullptr;
    this->screenW = 0;
    this->screenH = 0;
    
    this->g_alpha = 0;
    this->g_fadingOut = false;
    
    this->hoveredIndex = -1;

    this->isCountingDown = false;
    this->countdownSeconds = 0;
    this->isRestartCountdown = false;

    this->config.Load();
    this->buttons[0] = {0, 0, 60, L"Restart", IDB_RESTARTPNG};
    this->buttons[1] = {0, 0, 60, L"Shutdown", IDB_SHUTDOWNPNG};
  }

  HINSTANCE hInst;

  // size
  int screenW;
  int screenH;

  // effects
  unsigned char g_alpha;
  bool g_fadingOut;

  // buttons
  short hoveredIndex;

  // count down
  bool isCountingDown;
  short countdownSeconds;
  bool isRestartCountdown;

  // ui
  WCHAR szTitle[MAX_LOADSTRING];
  WCHAR szWindowClass[MAX_LOADSTRING];

  Config config;
  ImageButton buttons[2];
};