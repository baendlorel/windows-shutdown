#pragma once
#include <vector>
#include "resource.h"
#include "framework.h"
#include "consts/effects.h"
#include "consts/core.h"
#include "app-page.h"

#include "config.h"
#include "components/menu-button.h"

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

    AppPage page;

    // size
    int screenW = 0;
    int screenH = 0;

    // effects

    // buttons
    short hoveredIndex = -1;

    // actions
    Action action = Action::None;
    short countdownSeconds = 0;

    // ui
    WCHAR szTitle[MAX_LOADSTRING] = L"";
    WCHAR szWindowClass[MAX_LOADSTRING] = L"";

    Config config;
    std::vector<MenuButton> buttons;

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
        // Add donate button only if configuration allows it
        if (config.donateButton == DonateButton::Show) {
            this->buttons.push_back(MenuButton(IDB_DONATEPNG, Action::Donate));
        }
        this->buttons.push_back(MenuButton(IDB_CONFIGPNG, Action::Config));
        this->buttons.push_back(MenuButton(IDB_LOCKPNG, Action::Lock));
        this->buttons.push_back(MenuButton(IDB_SLEEPPNG, Action::Sleep));
        this->buttons.push_back(MenuButton(IDB_RESTARTPNG, Action::Restart));
        this->buttons.push_back(MenuButton(IDB_SHUTDOWNPNG, Action::Shutdown));
    }
};
