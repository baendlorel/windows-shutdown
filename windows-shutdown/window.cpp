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
    appState.page.SetAlpha(0);  // start fully transparent
    // If config requests immediate action, initialize the immediate action
    // state before starting the fade-in so the first drawn frame shows the
    // countdown UI instead of the main menu.
    if (appState.config.isImmediate()) {
        appState.page.Start(Page::Countdown);

        // Start countdown immediately so the first painted frame shows the
        // countdown UI rather than the main menu, avoiding flicker.
        StartCountdown(hWnd, appState.config.action);
    } else {
        appState.page.Start(Page::Main);
    }
    SetTimer(hWnd, FADEIN_TIMER_ID, FRAME_TIME, NULL);
    return TRUE;
}

void HandleTimer(HWND hWnd, WPARAM wParam) {
    static auto& appState = AppState::GetInstance();
    auto alpha = appState.page.alpha;

    if (wParam == FADEIN_TIMER_ID) {
        appState.page.fading = true;
        int steps = FADEIN_DURATION / FRAME_TIME;
        BYTE step = (MAX_ALPHA + steps - 1) / steps;
        if (alpha < MAX_ALPHA) {
            appState.page.SetAlpha((alpha + step > MAX_ALPHA) ? MAX_ALPHA : alpha + step);
            UpdateLayered(hWnd);
            return;
        }
        KillTimer(hWnd, FADEIN_TIMER_ID);
        appState.page.fading = false;
        return;
    }

    if (wParam == FADEOUT_TIMER_ID) {
        appState.page.fading = true;
        int steps = FADEIN_DURATION / FRAME_TIME;
        BYTE step = (MAX_ALPHA + steps - 1) / steps;
        if (alpha > 0) {
            appState.page.SetAlpha((alpha < step) ? 0 : alpha - step);
            UpdateLayered(hWnd);
            return;
        }
        KillTimer(hWnd, FADEOUT_TIMER_ID);
        appState.page.fading = false;
        DestroyWindow(hWnd);
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

void HandleKeydown(HWND hWnd) {
    static auto& appState = AppState::GetInstance();
    if (appState.isCountingDown()) {
        CancelCountdown(hWnd);
        return;
    }
    if (appState.page.fading) {
        return;
    }
    appState.page.fading = true;
    SetTimer(hWnd, FADEOUT_TIMER_ID, FRAME_TIME, NULL);
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

void HandleLeftClick(HWND hWnd, LPARAM lParam) {
    static auto& appState = AppState::GetInstance();
    if (appState.isCountingDown()) {
        CancelCountdown(hWnd);
        return;
    }
    int mx = LOWORD(lParam);
    int my = HIWORD(lParam);
    bool hit = false;
    for (int i = 0; i < appState.buttons.size(); ++i) {
        if (!appState.buttons[i].MouseHit(mx, my)) {
            continue;
        }
        hit = true;
        // todo 编写donate功能
        switch (appState.buttons[i].action) {
            case Action::Donate:
                TriggerDonate(hWnd);
                break;
            case Action::Config:
                TriggerConfig(hWnd);
                break;
            case Action::Lock:
                TriggerLock(hWnd);
                break;
            case Action::Sleep:
                StartCountdown(hWnd, Action::Sleep);
                break;
            case Action::Restart:
                StartCountdown(hWnd, Action::Restart);
                break;
            case Action::Shutdown:
                StartCountdown(hWnd, Action::Shutdown);
                break;
            case Action::None:
                // do nothing
                break;
        }
        break;
    }
    if (!hit && !appState.page.fading) {
        appState.page.fading = true;
        SetTimer(hWnd, FADEOUT_TIMER_ID, FRAME_TIME, NULL);
    }
}

void HandleCancel(HWND hWnd) {
    static auto& appState = AppState::GetInstance();
    if (appState.isCountingDown()) {
        CancelCountdown(hWnd);
    }
    if (!appState.page.fading) {
        appState.page.fading = true;
        SetTimer(hWnd, FADEOUT_TIMER_ID, FRAME_TIME, NULL);
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CLOSE:
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
        case WM_SYSKEYDOWN:
            if (!AppState::GetInstance().page.fading) {
                HandleKeydown(hWnd);
            }
            break;
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
            if (!AppState::GetInstance().page.fading) {
                HandleLeftClick(hWnd, lParam);
            }
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}