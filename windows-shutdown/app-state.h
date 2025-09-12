#pragma once
#include <vector>
#include "resource.h"
#include "framework.h"
#include "consts/effects.h"
#include "consts/core.h"
#include "app-page.h"

#include "config.h"

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

   public:
    HINSTANCE hInst = nullptr;

    AppPage page;

    // size
    int screenW = 0;
    int screenH = 0;

    // mouse
    int mouseX = 0;
    int mouseY = 0;

    // todo 可能要在这里用来触发一些事件
    void SetMousePos(int x, int y) {
        this->mouseX = x;
        this->mouseY = y;
    }

    // actions
    Action action = Action::None;
    short countdownSeconds = 0;

    // ui
    WCHAR szTitle[MAX_LOADSTRING] = L"";
    WCHAR szWindowClass[MAX_LOADSTRING] = L"";

    Config config;

    bool isCountingDown() const {
        return countdownSeconds > 0;
    }

   private:
    AppState() = default;
};
