#pragma once
#include "framework.h"
#include "singleton.h"

#include "app.event.h"

class AppState {
    DELETE_COPY_AND_MOVE(AppState)

   private:
    AppState() {
        AppEvent::GetInstance().on(EventType::Redraw, [this]() { this->needRedraw = true; });
    };

   public:
    HINSTANCE hInst = nullptr;

    // control the redrawing
    bool needRedraw = false;

    // size
    int screenW = 0;
    int screenH = 0;

    // mouse
    int mouseX = 0;
    int mouseY = 0;

    // actions
    Action action = Action::None;
    short countdownSeconds = 0;

    // ui
    WCHAR szTitle[MAX_LOAD_STRING] = L"";
    WCHAR szWindowClass[MAX_LOAD_STRING] = L"";

   public:
    bool isCountingDown() const {
        return countdownSeconds > 0;
    }

    // Will trigger MouseMove
    void SetMousePos(int x, int y) {
        static auto& appEvent = AppEvent::GetInstance();
        this->mouseX = x;
        this->mouseY = y;

        appEvent.emit(EventType::MouseMove);
    }
};
