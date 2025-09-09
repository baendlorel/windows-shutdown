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
        return countdownSeconds > 0;
    }

   private:
    AppState() {
        // Initialize buttons based on menuButtons configuration
        for (const auto& action : config.menuButtons) {
            switch (action) {
                case Action::Donate:
                    this->buttons.push_back(MenuButton(IDB_DONATEPNG, Action::Donate));
                    break;
                case Action::Config:
                    this->buttons.push_back(MenuButton(IDB_CONFIGPNG, Action::Config));
                    break;
                case Action::Lock:
                    this->buttons.push_back(MenuButton(IDB_LOCKPNG, Action::Lock));
                    break;
                case Action::Sleep:
                    this->buttons.push_back(MenuButton(IDB_SLEEPPNG, Action::Sleep));
                    break;
                case Action::Restart:
                    this->buttons.push_back(MenuButton(IDB_RESTARTPNG, Action::Restart));
                    break;
                case Action::Shutdown:
                    this->buttons.push_back(MenuButton(IDB_SHUTDOWNPNG, Action::Shutdown));
                    break;
                default:
                    // Skip invalid actions
                    break;
            }
        }
    }
};
