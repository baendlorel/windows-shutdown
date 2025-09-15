#pragma once
#include "framework.h"
#include "utils.class.h"

#include "app.event.h"

class AppState {
    DELETE_COPY_AND_MOVE(AppState)

   private:
    AppState() {
        AppEvent::get_instance().on(app::EventType::Redraw, [this]() { this->need_redraw = true; });
    }

   public:
    // ReSharper disable once CppInconsistentNaming
    HINSTANCE hInst = nullptr;

    // control the redrawing
    bool need_redraw = false;

    // size
    int screen_w = 0;
    int screen_h = 0;

    // mouse
    int mouse_x = 0;
    int mouse_y = 0;

    // actions
    app::Action action = app::Action::None;
    short countdown_seconds = 0;

    // ui
    // ReSharper disable once CppInconsistentNaming
    WCHAR szTitle[app::MAX_LOAD_STRING] = L"";

    // ReSharper disable once CppInconsistentNaming
    WCHAR szWindowClass[app::MAX_LOAD_STRING] = L"";

   public:
    [[nodiscard]] bool is_counting_down() const {
        return countdown_seconds > 0;
    }

    // Will trigger MouseMove
    void set_mouse_pos(const int x, const int y) {
        static auto& appEvent = AppEvent::get_instance();
        this->mouse_x = x;
        this->mouse_y = y;

        appEvent.emit(app::EventType::MouseMove);
    }
};
