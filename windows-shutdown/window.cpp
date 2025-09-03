#include "window.h"

#include <thread>

#include "Resource.h"
#include "app-state.h"
#include "consts.h"
#include "controller.h"
#include "framework.h"
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

BOOL InitInstance(int nCmdShow) {
    auto& appState = AppState::GetInstance();
    appState.screenW = GetSystemMetrics(SM_CXSCREEN);
    appState.screenH = GetSystemMetrics(SM_CYSCREEN);
    HWND hWnd = CreateWindowExW(WS_EX_LAYERED, appState.szWindowClass, appState.szTitle, WS_POPUP,
                                0, 0, appState.screenW, appState.screenH, nullptr, nullptr,
                                appState.hInst, nullptr);
    if (!hWnd) {
        auto& i18n = I18N::GetInstance();
        MessageBoxW(nullptr, i18n.Get(I18NKey::ErrorCreateWindow).c_str(),
                    i18n.Get(I18NKey::ErrorTitle).c_str(), MB_ICONERROR);
        return FALSE;
    }
    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);
    appState.g_alpha = 0;
    SetTimer(hWnd, FADEIN_TIMER_ID, FADEIN_INTERVAL, NULL);
    return TRUE;
}

void HandleTimer(HWND hWnd, WPARAM wParam) {
    static auto& appState = AppState::GetInstance();
    auto alpha = appState.g_alpha;

    if (wParam == FADEIN_TIMER_ID) {
        appState.fadeState = FadeState::FadingIn;
        BYTE targetAlpha = 255;
        int steps = FADEIN_DURATION / FADEIN_INTERVAL;
        BYTE step = (targetAlpha + steps - 1) / steps;
        if (alpha < targetAlpha) {
            appState.g_alpha = (alpha + step > targetAlpha) ? targetAlpha : alpha + step;
            UpdateLayered(hWnd);
            return;
        }
        KillTimer(hWnd, FADEIN_TIMER_ID);
        appState.fadeState = FadeState::None;
        return;
    }

    if (wParam == FADEOUT_TIMER_ID) {
        appState.fadeState = FadeState::FadingOut;
        int steps = FADEIN_DURATION / FADEIN_INTERVAL;
        BYTE step = (255 + steps - 1) / steps;
        if (alpha > 0) {
            appState.g_alpha = (alpha < step) ? 0 : alpha - step;
            UpdateLayered(hWnd);
            return;
        }
        KillTimer(hWnd, FADEOUT_TIMER_ID);
        appState.fadeState = FadeState::None;
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
        if (appState.isSleepCountdown()) {
            // Close the program first, then sleep
            PostMessage(hWnd, WM_CLOSE, 0, 0);
            // Use a separate thread to handle sleep after window closes
            std::thread([]() {
                Sleep(500);  // Give time for window to close
                ExecuteSleep();
            }).detach();
            return;
        }

        if (appState.isRestartCountdown()) {
            ExecuteRestart();
            return;
        }

        ExecuteShutdown();
        return;
    }
}

void HandleKeydown(HWND hWnd) {
    static auto& appState = AppState::GetInstance();
    if (appState.isCountingDown()) {
        CancelCountdown(hWnd);
        return;
    }
    if (appState.fadeState != FadeState::None) {
        return;
    }
    appState.fadeState = FadeState::FadingOut;
    SetTimer(hWnd, FADEOUT_TIMER_ID, FADEIN_INTERVAL, NULL);
}

void HandleMoustMove(HWND hWnd, LPARAM lParam) {
    static auto& appState = AppState::GetInstance();
    if (appState.isCountingDown()) {
        return;  // Ignore mouse move during countdown
    }
    int mx = LOWORD(lParam);
    int my = HIWORD(lParam);
    int newHover = -1;
    for (int i = 0; i < 5; ++i) {
        if (appState.buttons[i].mouseHit(mx, my)) {
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
    for (int i = 0; i < 5; ++i) {
        if (!appState.buttons[i].mouseHit(mx, my)) {
            continue;
        }
        hit = true;
        switch (static_cast<Button>(i)) {
            case Button::Config:
                TriggerConfig(hWnd);
                break;
            case Button::Lock:
                TriggerLock(hWnd);
                break;
            case Button::Sleep:
                TriggerSleep(hWnd);
                break;
            case Button::Restart:
                TriggerRestart(hWnd);
                break;
            case Button::Shutdown:
                TriggerShutdown(hWnd);
                break;
        }
        break;
    }
    if (!hit && appState.fadeState == FadeState::None) {
        appState.fadeState = FadeState::FadingOut;
        SetTimer(hWnd, FADEOUT_TIMER_ID, FADEIN_INTERVAL, NULL);
    }
}

void HandleCancel(HWND hWnd) {
    static auto& appState = AppState::GetInstance();
    if (appState.isCountingDown()) {
        CancelCountdown(hWnd);
    }
    if (appState.fadeState == FadeState::None) {
        appState.fadeState = FadeState::FadingOut;
        SetTimer(hWnd, FADEOUT_TIMER_ID, FADEIN_INTERVAL, NULL);
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    auto& appState = AppState::GetInstance();
    switch (message) {
        case WM_TIMER:
            HandleTimer(hWnd, wParam);
            break;
        case WM_PAINT:
            UpdateLayered(hWnd);
            break;
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            HandleKeydown(hWnd);
            break;
        case WM_MOUSEMOVE:
            HandleMoustMove(hWnd, lParam);
            break;
        case WM_LBUTTONDOWN:
            HandleLeftClick(hWnd, lParam);
            break;
        case WM_CLOSE:
            HandleCancel(hWnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}