#include "window.h"

#include "consts.app.h"
#include "style.fade.h"
#include "app.core.h"

#include "controller.h"
#include "index.h"

ATOM window::my_register_class() {
    WNDCLASSEXW window_class_ex_w;
    window_class_ex_w.cbSize = sizeof(WNDCLASSEX);
    window_class_ex_w.style = CS_HREDRAW | CS_VREDRAW;
    window_class_ex_w.lpfnWndProc = window::WndProc;
    window_class_ex_w.cbClsExtra = 0;
    window_class_ex_w.cbWndExtra = 0;
    window_class_ex_w.hInstance = app::state.hInst;
    window_class_ex_w.hIcon = LoadIcon(app::state.hInst, MAKEINTRESOURCE(IDI_MAINICON));
    window_class_ex_w.hCursor = LoadCursor(nullptr, IDC_ARROW);
    window_class_ex_w.hbrBackground = nullptr;
    window_class_ex_w.lpszMenuName = nullptr;
    window_class_ex_w.lpszClassName = app::state.szWindowClass;
    window_class_ex_w.hIconSm = LoadIcon(app::state.hInst, MAKEINTRESOURCE(IDI_MAINICON));
    return RegisterClassExW(&window_class_ex_w);
}

void window::register_menu_button_click_callback() {
    auto& menu = Index::get_instance().home.menu;
    for (auto& button : menu) {
        switch (button.action) {
            case app::Action::Donate:
                button.on(app::EventType::MouseClick, controller::trigger_donate);
                break;
            case app::Action::Config:
                button.on_click(controller::trigger_config);
                break;
            case app::Action::Lock:
                button.on_click(controller::trigger_lock);
                break;
            case app::Action::Sleep:
                button.on_click(
                    [](const HWND hWnd) { controller::start_countdown(hWnd, app::Action::Sleep); });
                break;
            case app::Action::Restart:
                button.on_click([](const HWND hWnd) {
                    controller::start_countdown(hWnd, app::Action::Restart);
                });
                break;
            case app::Action::Shutdown:
                button.on_click([](const HWND hWnd) {
                    controller::start_countdown(hWnd, app::Action::Shutdown);
                });
                break;
            case app::Action::None:
                // do nothing
                break;
        }
    }
}

// int nCmdShow
BOOL window::init_instance(int) {
    app::state.screen_w = GetSystemMetrics(SM_CXSCREEN);
    app::state.screen_h = GetSystemMetrics(SM_CYSCREEN);
    const HWND hWnd = CreateWindowExW(WS_EX_LAYERED, app::state.szWindowClass, app::state.szTitle,
                                      WS_POPUP, 0, 0, app::state.screen_w, app::state.screen_h,
                                      nullptr, nullptr, app::state.hInst, nullptr);
    if (!hWnd) {
        MessageBoxW(nullptr, app::i18n.ErrCreateWindow.c_str(), app::i18n.ErrTitle.c_str(),
                    MB_ICONERROR);
        return FALSE;
    }
    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    // ui
    Index::get_instance().home.init_menu();
    register_menu_button_click_callback();

    // If config requests immediate action, initialize the immediate action
    // state before starting the fade-in so the first drawn frame shows the
    // countdown UI instead of the main menu.
    if (app::config.is_immediate()) {
        controller::start_countdown(hWnd, app::config.action);
    } else {
        app::page.start(app::Page::Home, hWnd);
    }
    return TRUE;
}

void window::handle_timer(HWND hWnd, WPARAM wParam) {
    const auto alpha = app::page.alpha;

    if (wParam == fade::TIMER_ID) {
        constexpr int steps = fade::DURATION / fade::FRAME_TIME;
        constexpr BYTE step = (fade::MAX_ALPHA + steps - 1) / steps;

        if (alpha < fade::MAX_ALPHA) {
            app::page.set_alpha(alpha + step > fade::MAX_ALPHA ? fade::MAX_ALPHA : alpha + step);
            render->update_layered(hWnd);
            return;
        }

        KillTimer(hWnd, fade::TIMER_ID);

        // If fading out to close, close now
        if (app::page.current == app::Page::None) {
            DestroyWindow(hWnd);
            return;
        }
        return;
    }

    if (wParam == app::COUNTDOWN_TIMER_ID && app::state.is_counting_down()) {
        app::state.countdown_seconds--;
        if (app::state.countdown_seconds > 0) {
            // Redraw to update countdown
            render->update_layered(hWnd);
            return;
        }

        controller::cancel_countdown(hWnd);
        controller::execute_action(hWnd, app::state.action);
    }
}

void window::handle_cancel(HWND hWnd) {
    if (app::state.is_counting_down()) {
        controller::cancel_countdown(hWnd);
        return;
    }

    if (app::page.current == app::Page::Home) {
        app::page.start(app::Page::None, hWnd);
    } else {
        app::page.start(app::Page::Home, hWnd);
    }
}

void window::handle_mouse_move(HWND hWnd, LPARAM lParam) {
    app::state.set_mouse_pos(LOWORD(lParam), HIWORD(lParam));
}

void window::handle_click(HWND hWnd, LPARAM lParam) {
    static auto& menu = Index::get_instance().home.menu;
    if (app::state.is_counting_down()) {
        controller::cancel_countdown(hWnd);
        return;
    }

    // if clicked, return after handling
    if (app::page.current == app::Page::Home) {
        const int mx = LOWORD(lParam);
        const int my = HIWORD(lParam);
        for (auto& b : menu) {
            if (!b.mouse_hit(mx, my)) {
                continue;
            }
            b.trigger_click(hWnd);
            return;
        }
    }

    // If not clicking on any button, return to main page or exit
    handle_cancel(hWnd);
}

LRESULT CALLBACK window::WndProc(const HWND hWnd, const UINT message, WPARAM wParam,
                                 LPARAM lParam) {
    switch (message) {
        case WM_CLOSE:
            // Treat close like pressing Esc: if on Home start fade-out to close,
            // otherwise navigate back to Home. This lets taskbar "Close" actually
            // begin the shutdown/fade-out sequence instead of only cancelling.
            handle_cancel(hWnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_TIMER:
            handle_timer(hWnd, wParam);
            break;
        case WM_PAINT:
            render->update_layered(hWnd);
            break;
        case WM_MOUSEMOVE:
            handle_mouse_move(hWnd, lParam);
            break;
        case WM_KEYDOWN:
            // Must not be fading
            if (!app::page.fading && wParam == VK_ESCAPE) {
                handle_cancel(hWnd);
            }
            // & after test, it is found that activate really works
            if (!app::page.fading && wParam == VK_F5) {
                render->update_layered(hWnd);
            }
            break;
        case WM_RBUTTONDOWN:
            // Must not be fading
            if (!app::page.fading) {
                handle_cancel(hWnd);
            }
            break;
        case WM_LBUTTONDOWN:
            if (!app::page.fading) {
                handle_click(hWnd, lParam);
            }
            break;
        default:
            // mousemove -> menu button hover -> before != after -> redraw
            if (app::state.need_redraw) {
                render->update_layered(hWnd);
            }
            return DefWindowProc(hWnd, message, wParam, lParam);
    }

    if (app::state.need_redraw) {
        render->update_layered(hWnd);
    }
    return 0;
}
