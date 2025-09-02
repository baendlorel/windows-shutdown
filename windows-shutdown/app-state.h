#pragma once
#include "Resource.h"
#include "config.h"
#include "consts.h"
#include "framework.h"
#include "image-button.h"

class AppState {
   public:
    static AppState& getInstance() {
        static AppState instance;
        return instance;
    }

   public:
    AppState(const AppState&) = delete;
    AppState& operator=(const AppState&) = delete;

    AppState(AppState&&) = delete;
    AppState& operator=(AppState&&) = delete;

    HINSTANCE hInst;

    // size
    int screenW;
    int screenH;

    // effects
    unsigned char g_alpha;
    bool g_fadingOut;

    // buttons
    short hoveredIndex;

    // actions
    Action action;
    bool isCountingDown;
    bool isRestartCountdown;
    bool isSleepCountdown;
    short countdownSeconds;

    // ui
    WCHAR szTitle[MAX_LOADSTRING];
    WCHAR szWindowClass[MAX_LOADSTRING];

    Config config;
    ImageButton buttons[BUTTON_COUNT];

   private:
    AppState()
        :  // windows
          hInst(nullptr),
          screenW(0),
          screenH(0),
          // fade settings
          g_alpha(0),
          g_fadingOut(false),

          // buttons and actions
          hoveredIndex(-1),
          action(Action::None),
          isCountingDown(false),
          isRestartCountdown(false),
          isSleepCountdown(false),
          countdownSeconds(0) {
        // window info
        this->szTitle[0] = L'\0';
        this->szWindowClass[0] = L'\0';

        // buttons
        this->buttons[0].resId = IDB_CONFIGPNG;
        this->buttons[1].resId = IDB_LOCKPNG;
        this->buttons[2].resId = IDB_SLEEPPNG;
        this->buttons[3].resId = IDB_RESTARTPNG;
        this->buttons[4].resId = IDB_SHUTDOWNPNG;
    }
};