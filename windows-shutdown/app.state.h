#pragma once
#include <vector>
#include "framework.h"
#include "singleton.h"

#include "style.fade.h"
#include "consts.app.h"

#include "app.page.h"
#include "app.config.h"

class AppState {
    SINGLETON(AppState)

   public:
    HINSTANCE hInst = nullptr;

    // size
    int screenW = 0;
    int screenH = 0;

    // mouse
    int mouseX = 0;
    int mouseY = 0;

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

    bool isCountingDown() const {
        return countdownSeconds > 0;
    }
};
