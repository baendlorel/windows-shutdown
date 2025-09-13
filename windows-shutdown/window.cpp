#include "window.h"

#include "consts.app.h"
#include "style.fade.h"
#include "app.core.h"

#include "controller.h"
#include "render.h"
#include "index.h"

ATOM Window::MyRegisterClass() {
    auto& appState = AppState::GetInstance();
    WNDCLASSEXW wcex{};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = appState.hInst;
    wcex.hIcon = LoadIcon(appState.hInst, MAKEINTRESOURCE(IDI_MAINICON));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = NULL;
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = appState.szWindowClass;
    wcex.hIconSm = LoadIcon(appState.hInst, MAKEINTRESOURCE(IDI_MAINICON));
    return RegisterClassExW(&wcex);
}

void Window::RegisterMenuButtonClickCallback() {
    auto& menu = Index::GetInstance().home.menu;
    auto& ctrl = controller;
    for (auto& button : menu) {
        switch (button.action) {
            case Action::Donate:
                button.OnClick([&ctrl](HWND hWnd) { ctrl.TriggerDonate(hWnd); });
                break;
            case Action::Config:
                button.OnClick([&ctrl](HWND hWnd) { ctrl.TriggerConfig(hWnd); });
                break;
            case Action::Lock:
                button.OnClick([&ctrl](HWND hWnd) { ctrl.TriggerLock(hWnd); });
                break;
            case Action::Sleep:
                button.OnClick([&ctrl](HWND hWnd) { ctrl.StartCountdown(hWnd, Action::Sleep); });
                break;
            case Action::Restart:
                button.OnClick([&ctrl](HWND hWnd) { ctrl.StartCountdown(hWnd, Action::Restart); });
                break;
            case Action::Shutdown:
                button.OnClick([&ctrl](HWND hWnd) { ctrl.StartCountdown(hWnd, Action::Shutdown); });
                break;
            case Action::None:
                // do nothing
                break;
        }
    }
}

// int nCmdShow
BOOL Window::InitInstance(int) {
    auto& app = App::GetInstance();
    app.state.screenW = GetSystemMetrics(SM_CXSCREEN);
    app.state.screenH = GetSystemMetrics(SM_CYSCREEN);
    HWND hWnd = CreateWindowExW(WS_EX_LAYERED, app.state.szWindowClass, app.state.szTitle, WS_POPUP,
                                0, 0, app.state.screenW, app.state.screenH, nullptr, nullptr,
                                app.state.hInst, nullptr);
    if (!hWnd) {
        MessageBoxW(nullptr, app.i18n.ErrCreateWindow.c_str(), app.i18n.ErrTitle.c_str(),
                    MB_ICONERROR);
        return FALSE;
    }
    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    // ui
    Index::GetInstance().home.initMenu();
    RegisterMenuButtonClickCallback();

    // If config requests immediate action, initialize the immediate action
    // state before starting the fade-in so the first drawn frame shows the
    // countdown UI instead of the main menu.
    if (app.config.IsImmediate()) {
        controller.StartCountdown(hWnd, app.config.action);
    } else {
        app.page.Start(Page::Home, hWnd);
    }
    return TRUE;
}

void Window::HandleTimer(HWND hWnd, WPARAM wParam) {
    static auto& app = App::GetInstance();
    auto alpha = app.page.alpha;

    if (wParam == FADE_TIMER_ID) {
        int steps = FADE_DURATION / FRAME_TIME;
        BYTE step = (MAX_ALPHA + steps - 1) / steps;

        if (alpha < MAX_ALPHA) {
            app.page.SetAlpha((alpha + step > MAX_ALPHA) ? MAX_ALPHA : alpha + step);
            render.UpdateLayered(hWnd);
            return;
        }

        KillTimer(hWnd, FADE_TIMER_ID);

        // If fading out to close, close now
        if (app.page.current == Page::None) {
            DestroyWindow(hWnd);
            return;
        }
        return;
    }

    if (wParam == COUNTDOWN_TIMER_ID && app.state.isCountingDown()) {
        app.state.countdownSeconds--;
        if (app.state.countdownSeconds > 0) {
            // Redraw to update countdown
            render.UpdateLayered(hWnd);
            return;
        }

        controller.CancelCountdown(hWnd);
        controller.ExecuteAction(hWnd, app.state.action);
        return;
    }
}

void Window::HandleCancel(HWND hWnd) {
    static auto& app = App::GetInstance();
    if (app.state.isCountingDown()) {
        controller.CancelCountdown(hWnd);
        return;
    }

    if (app.page.current == Page::Home) {
        app.page.Start(Page::None, hWnd);
    } else {
        app.page.Start(Page::Home, hWnd);
    }
}

void Window::HandleMoustMove(HWND hWnd, LPARAM lParam) {
    static auto& app = App::GetInstance();
    app.state.mouseX = LOWORD(lParam);
    app.state.mouseY = HIWORD(lParam);
}

void Window::HandleClick(HWND hWnd, LPARAM lParam) {
    static auto& app = App::GetInstance();
    static auto& menu = Index::GetInstance().home.menu;
    if (app.state.isCountingDown()) {
        controller.CancelCountdown(hWnd);
        return;
    }

    // if clicked, return after handling
    if (app.page.current == Page::Home) {
        int mx = LOWORD(lParam);
        int my = HIWORD(lParam);
        for (int i = 0; i < menu.size(); ++i) {
            if (!menu[i].MouseHit(mx, my)) {
                continue;
            }
            menu[i].TriggerClick(hWnd);
            return;
        }
    }

    // If not clicking on any button, return to main page or exit
    HandleCancel(hWnd);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static auto& app = App::GetInstance();
    static auto& window = Window::GetInstance();
    static auto& render = Render::GetInstance();

    switch (message) {
        case WM_CLOSE:
            // Treat close like pressing Esc: if on Home start fade-out to close,
            // otherwise navigate back to Home. This lets taskbar "Close" actually
            // begin the shutdown/fade-out sequence instead of only cancelling.
            window.HandleCancel(hWnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_TIMER:
            window.HandleTimer(hWnd, wParam);
            break;
        case WM_PAINT:
            render.UpdateLayered(hWnd);
            break;
        case WM_MOUSEMOVE:
            window.HandleMoustMove(hWnd, lParam);
            break;
        case WM_KEYDOWN:
            // Must not be fading
            if (!app.page.fading && wParam == VK_ESCAPE) {
                window.HandleCancel(hWnd);
            }
            // & after test, it is found that activate really works
            if (!app.page.fading && wParam == VK_F5) {
                render.UpdateLayered(hWnd);
            }
            break;
        case WM_RBUTTONDOWN:
            // Must not be fading
            if (!app.page.fading) {
                window.HandleCancel(hWnd);
            }
            break;
        case WM_LBUTTONDOWN:
            if (!app.page.fading) {
                window.HandleClick(hWnd, lParam);
            }
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
