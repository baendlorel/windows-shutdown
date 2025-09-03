#pragma once
#include "Resource.h"
#include "config.h"
#include "consts.h"
#include "framework.h"
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

    HINSTANCE hInst;

    // size
    int screenW;
    int screenH;

    // effects
    unsigned char g_alpha;

    // buttons
    short hoveredIndex;

    // actions
    Action action;
    short countdownSeconds;

    // ui
    WCHAR szTitle[MAX_LOADSTRING];
    WCHAR szWindowClass[MAX_LOADSTRING];

    Config config;
    ImageButton buttons[BUTTON_COUNT];

    FadeState fadeState = FadeState::None;

    bool isCountingDown() const {
        return action != Action::None;
    }
    bool isRestartCountdown() const {
        return action == Action::Restart;
    }
    bool isSleepCountdown() const {
        return action == Action::Sleep;
    }

    std::wstring GetExeName() const {
        wchar_t exePath[MAX_PATH] = {0};
        GetModuleFileNameW(NULL, exePath, MAX_PATH);
        std::wstring path(exePath);
        size_t pos = path.find_last_of(L"\\/");
        if (pos != std::wstring::npos) {
            return path.substr(pos + 1);
        }
        return L"";
    }

   private:
    AppState()
        :  // windows
          hInst(nullptr),
          screenW(0),
          screenH(0),
          // fade settings
          g_alpha(0),
          fadeState(FadeState::None),

          // buttons and actions
          hoveredIndex(-1),
          action(Action::None),
          countdownSeconds(0) {
        // window info
        this->szTitle[0] = L'\0';
        this->szWindowClass[0] = L'\0';

        // buttons
        this->buttons[static_cast<int>(Button::Config)].resId = IDB_CONFIGPNG;
        this->buttons[static_cast<int>(Button::Lock)].resId = IDB_LOCKPNG;
        this->buttons[static_cast<int>(Button::Sleep)].resId = IDB_SLEEPPNG;
        this->buttons[static_cast<int>(Button::Restart)].resId = IDB_RESTARTPNG;
        this->buttons[static_cast<int>(Button::Shutdown)].resId = IDB_SHUTDOWNPNG;
    }
};
