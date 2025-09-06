#pragma once
#include "resource.h"
#include "framework.h"
#include "consts/effects.h"
#include "config.h"
#include "image-button.h"

class AppState {
   public:
    static AppState& GetInstance() {
        static AppState instance;
        return instance;
    }

   public:
    AppState(const AppState&) = delete;
    AppState& operator=(const AppState&) = delete;

    AppState(AppState&&) = delete;
    AppState& operator=(AppState&&) = delete;

    HINSTANCE hInst = nullptr;

    // size
    int screenW = 0;
    int screenH = 0;

    // effects
    FadeState fadeState = FadeState::None;
    unsigned char g_alpha = 0;

    // buttons
    short hoveredIndex = -1;

    // actions
    Action action = Action::None;
    short countdownSeconds = 0;

    // ui
    WCHAR szTitle[MAX_LOADSTRING] = L"";
    WCHAR szWindowClass[MAX_LOADSTRING] = L"";

    Config config;
    ImageButton buttons[BUTTON_COUNT];

    bool isCountingDown() const {
        return action != Action::None;
    }
    bool isRestartCountdown() const {
        return action == Action::Restart;
    }
    bool isSleepCountdown() const {
        return action == Action::Sleep;
    }

   private:
    AppState() {
        // buttons
        this->buttons[static_cast<int>(Button::Donate)].resId = IDB_DONATEPNG;
        this->buttons[static_cast<int>(Button::Config)].resId = IDB_CONFIGPNG;
        this->buttons[static_cast<int>(Button::Lock)].resId = IDB_LOCKPNG;
        this->buttons[static_cast<int>(Button::Sleep)].resId = IDB_SLEEPPNG;
        this->buttons[static_cast<int>(Button::Restart)].resId = IDB_RESTARTPNG;
        this->buttons[static_cast<int>(Button::Shutdown)].resId = IDB_SHUTDOWNPNG;
    }
};
