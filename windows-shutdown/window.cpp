#include "window.h"

#include "consts/core.h"
#include "consts/effects.h"

#include "resource.h"
#include "app-state.h"
#include "controller.h"
#include "render.h"
#include "i18n.h"

ATOM MyRegisterClass() {
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

void RegisterMenuButtonClickCallback() {
    auto& appState = AppState::GetInstance();
    for (auto& button : appState.buttons) {
        switch (button.action) {
            case Action::Donate:
                button.OnClick(TriggerDonate);
                break;
            case Action::Config:
                button.OnClick(TriggerConfig);
                break;
            case Action::Lock:
                button.OnClick(TriggerLock);
                break;
            case Action::Sleep:
                button.OnClick([](HWND hWnd) { StartCountdown(hWnd, Action::Sleep); });
                break;
            case Action::Restart:
                button.OnClick([](HWND hWnd) { StartCountdown(hWnd, Action::Restart); });
                break;
            case Action::Shutdown:
                button.OnClick([](HWND hWnd) { StartCountdown(hWnd, Action::Shutdown); });
                break;
            case Action::None:
                // do nothing
                break;
        }
    }
}

// int nCmdShow
BOOL InitInstance(int) {
    auto& appState = AppState::GetInstance();
    appState.screenW = GetSystemMetrics(SM_CXSCREEN);
    appState.screenH = GetSystemMetrics(SM_CYSCREEN);
    HWND hWnd = CreateWindowExW(WS_EX_LAYERED, appState.szWindowClass, appState.szTitle, WS_POPUP,
                                0, 0, appState.screenW, appState.screenH, nullptr, nullptr,
                                appState.hInst, nullptr);
    if (!hWnd) {
        auto& i18n = I18N::GetInstance();
        MessageBoxW(nullptr, i18n.ErrCreateWindow.c_str(), i18n.ErrTitle.c_str(), MB_ICONERROR);
        return FALSE;
    }
    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    RegisterMenuButtonClickCallback();

    // If config requests immediate action, initialize the immediate action
    // state before starting the fade-in so the first drawn frame shows the
    // countdown UI instead of the main menu.
    if (appState.config.isImmediate()) {
        StartCountdown(hWnd, appState.config.action);
    } else {
        appState.page.Start(Page::Home, hWnd);
    }
    return TRUE;
}

void HandleTimer(HWND hWnd, WPARAM wParam) {
    static auto& appState = AppState::GetInstance();
    auto alpha = appState.page.alpha;

    if (wParam == FADE_TIMER_ID) {
        int steps = FADE_DURATION / FRAME_TIME;
        BYTE step = (MAX_ALPHA + steps - 1) / steps;
        if (alpha < MAX_ALPHA) {
            appState.page.SetAlpha((alpha + step > MAX_ALPHA) ? MAX_ALPHA : alpha + step);
            UpdateLayered(hWnd);
            return;
        }

        KillTimer(hWnd, FADE_TIMER_ID);

        // If fading out to close, close now
        if (appState.page.current == Page::None) {
            DestroyWindow(hWnd);
            return;
        }
        return;
    }

    if (wParam == COUNTDOWN_TIMER_ID && appState.isCountingDown()) {
        appState.countdownSeconds--;
        if (appState.countdownSeconds > 0) {
            // Redraw to update countdown
            UpdateLayered(hWnd);
            return;
        }

        CancelCountdown(hWnd);
        ExecuteAction(hWnd, appState.action);
        return;
    }
}

void HandleCancel(HWND hWnd) {
    static auto& appState = AppState::GetInstance();
    if (appState.isCountingDown()) {
        CancelCountdown(hWnd);
        return;
    }

    if (appState.page.current == Page::Home) {
        appState.page.Start(Page::None, hWnd);
    } else {
        appState.page.Start(Page::Home, hWnd);
    }
}

void HandleMoustMove(HWND hWnd, LPARAM lParam) {
    static auto& appState = AppState::GetInstance();
    if (appState.isCountingDown()) {
        return;  // Ignore mouse move during countdown
    }
    int mx = LOWORD(lParam);
    int my = HIWORD(lParam);
    int newHover = -1;
    for (int i = 0; i < appState.buttons.size(); ++i) {
        if (appState.buttons[i].MouseHit(mx, my)) {
            newHover = i;
            break;
        }
    }

    if (newHover != appState.hoveredIndex) {
        appState.hoveredIndex = newHover;
        UpdateLayered(hWnd);
    }
}

void HandleClick(HWND hWnd, LPARAM lParam) {
    static auto& appState = AppState::GetInstance();
    if (appState.isCountingDown()) {
        CancelCountdown(hWnd);
        return;
    }

    // if clicked, return after handling
    if (appState.page.current == Page::Home) {
        int mx = LOWORD(lParam);
        int my = HIWORD(lParam);
        for (int i = 0; i < appState.buttons.size(); ++i) {
            if (!appState.buttons[i].MouseHit(mx, my)) {
                continue;
            }
            appState.buttons[i].onClickCallback(hWnd);
            return;
        }
    }

    // If not clicking on any button, return to main page or exit
    HandleCancel(hWnd);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static auto& appState = AppState::GetInstance();
    switch (message) {
        case WM_CLOSE:
            // Treat close like pressing Esc: if on Home start fade-out to close,
            // otherwise navigate back to Home. This lets taskbar "Close" actually
            // begin the shutdown/fade-out sequence instead of only cancelling.
            HandleCancel(hWnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_TIMER:
            HandleTimer(hWnd, wParam);
            break;
        case WM_PAINT:
            UpdateLayered(hWnd);
            break;
        case WM_MOUSEMOVE:
            HandleMoustMove(hWnd, lParam);
            break;
        case WM_KEYDOWN:
            // Must not be fading
            if (!appState.page.fading && wParam == VK_ESCAPE) {
                HandleCancel(hWnd);
            }
            break;
        case WM_RBUTTONDOWN:
            // Must not be fading
            if (!appState.page.fading) {
                HandleCancel(hWnd);
            }
            break;
        case WM_LBUTTONDOWN:
            if (!appState.page.fading) {
                HandleClick(hWnd, lParam);
            }
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}