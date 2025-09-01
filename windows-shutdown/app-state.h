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
    ImageButton buttons[5];

   private:
    AppState() {
        this->hInst = nullptr;
        this->screenW = 0;
        this->screenH = 0;

        this->g_alpha = 0;
        this->g_fadingOut = false;

        this->hoveredIndex = -1;

        this->action = Action::None;
        this->isCountingDown = false;
        this->countdownSeconds = 0;
        this->isRestartCountdown = false;
        this->isSleepCountdown = false;

        this->szTitle[0] = L'\0';
        this->szWindowClass[0] = L'\0';

        this->buttons[0] = {0, 0, 60, L"Config", IDB_CONFIGPNG};
        this->buttons[1] = {0, 0, 60, L"Lock", IDB_LOCKPNG};
        this->buttons[2] = {0, 0, 60, L"Sleep", IDB_SLEEPPNG};
        this->buttons[3] = {0, 0, 60, L"Restart", IDB_RESTARTPNG};
        this->buttons[4] = {0, 0, 60, L"Shutdown", IDB_SHUTDOWNPNG};
    }
};