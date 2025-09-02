#include "controller.h"

#include <powrprof.h>
#include <shellapi.h>

#include "app-state.h"
#include "config.h"
#include "framework.h"
#include "render.h"

#pragma comment(lib, "PowrProf.lib")

void ExecuteRestart() {
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;
    OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);

    wchar_t msg[] = L"Restarting...";
    InitiateSystemShutdownEx(NULL, msg, 0, TRUE, TRUE, SHTDN_REASON_MAJOR_OTHER);
}

void ExecuteShutdown() {
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;
    OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);

    wchar_t msg[] = L"Shutdown...";
    InitiateSystemShutdownEx(NULL, msg, 0, TRUE, FALSE, SHTDN_REASON_MAJOR_OTHER);
}

void ExecuteSleep() {
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        if (LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid)) {
            tkp.PrivilegeCount = 1;
            tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
            AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
        }
        CloseHandle(hToken);
    }

    SetSuspendState(FALSE, FALSE, FALSE);
}

void ExecuteLock() {
    LockWorkStation();
}

void StartCountdown(HWND hWnd, Action action) {
    auto& appState = AppState::GetInstance();
    if (appState.config.delay <= 0) {
        switch (action) {
            case Action::Sleep:
                // For sleep, close the program first then sleep
                PostMessage(hWnd, WM_CLOSE, 0, 0);
                // Use a small delay to ensure the program closes before sleeping
                Sleep(500);
                ExecuteSleep();
                break;
            case Action::Shutdown:
                ExecuteShutdown();
                break;
            case Action::Restart:
                ExecuteRestart();
                break;
            case Action::None:
            default:
                break;
        }
        return;
    }

    appState.action = action;
    appState.countdownSeconds = appState.config.delay;
    SetTimer(hWnd, COUNTDOWN_TIMER_ID, 1000, NULL);  // 1 second interval
    UpdateLayered(hWnd);                             // Redraw to show countdown
}

void CancelCountdown(HWND hWnd) {
    auto& appState = AppState::GetInstance();
    if (appState.isCountingDown()) {
        appState.action = Action::None;
        KillTimer(hWnd, COUNTDOWN_TIMER_ID);
        UpdateLayered(hWnd);  // Redraw to hide countdown
    }
}

void TriggerRestart(HWND hWnd) {
    StartCountdown(hWnd, Action::Restart);
}

void TriggerShutdown(HWND hWnd) {
    StartCountdown(hWnd, Action::Shutdown);
}

void TriggerSleep(HWND hWnd) {
    StartCountdown(hWnd, Action::Sleep);
}

void TriggerLock(HWND hWnd) {
    auto& appState = AppState::GetInstance();
    if (appState.fadeState != FadeState::None) return;
    appState.fadeState = FadeState::FadingOut;
    SetTimer(hWnd, FADEOUT_TIMER_ID, FADEIN_INTERVAL, NULL);
    ExecuteLock();
}

void TriggerConfig(HWND hWnd) {
    auto& appState = AppState::GetInstance();
    std::wstring configPath = GetConfigPath();
    ShellExecuteW(NULL, L"open", configPath.c_str(), NULL, NULL, SW_SHOWNORMAL);
    if (appState.fadeState != FadeState::None) return;
    appState.fadeState = FadeState::FadingOut;
    SetTimer(hWnd, FADEOUT_TIMER_ID, FADEIN_INTERVAL, NULL);
}

// todo 根据文件名决定是关机还是重启还是睡眠
void ActionByFileName() {
}