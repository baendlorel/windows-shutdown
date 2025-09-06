#pragma once
#include <vector>
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
    std::vector<ImageButton> buttons;

    bool isCountingDown() const {
        switch (action) {
            case Action::Restart:
                return true;
            case Action::Sleep:
                return true;
            case Action::Shutdown:
                return true;
            default:
                return false;
        }
    }

   private:
    AppState() {
        this->buttons.push_back(ImageButton(IDB_DONATEPNG, Action::Donate));
        this->buttons.push_back(ImageButton(IDB_CONFIGPNG, Action::Config));
        this->buttons.push_back(ImageButton(IDB_LOCKPNG, Action::Lock));
        this->buttons.push_back(ImageButton(IDB_SLEEPPNG, Action::Sleep));
        this->buttons.push_back(ImageButton(IDB_RESTARTPNG, Action::Restart));
        this->buttons.push_back(ImageButton(IDB_SHUTDOWNPNG, Action::Shutdown));
    }
};
