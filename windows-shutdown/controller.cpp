#include "controller.h"

#include <powrprof.h>
#include <shellapi.h>

#include "app-state.h"
#include "config.h"
#include "framework.h"
#include "render.h"
#include "i18n.h"

// msvc use: #pragma comment(lib, "PowrProf.lib")

void ExecuteRestart() {
    auto& i18n = I18N::GetInstance();
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        MessageBoxW(nullptr, i18n.ErrGetProcessTokenRestart.c_str(), i18n.ErrTitle.c_str(),
                    MB_ICONERROR);
        return;
    }
    if (!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid)) {
        MessageBoxW(nullptr, i18n.ErrLookupPrivilegeRestart.c_str(), i18n.ErrTitle.c_str(),
                    MB_ICONERROR);
        CloseHandle(hToken);
        return;
    }
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, static_cast<PTOKEN_PRIVILEGES>(nullptr), 0);
    CloseHandle(hToken);

    wchar_t msg[] = L"Restarting...";
    if (!InitiateSystemShutdownEx(NULL, msg, 0, TRUE, TRUE, SHTDN_REASON_MAJOR_OTHER)) {
        MessageBoxW(nullptr, i18n.ErrRestartFailed.c_str(), i18n.ErrTitle.c_str(), MB_ICONERROR);
    }
}

void ExecuteShutdown() {
    auto& i18n = I18N::GetInstance();
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        MessageBoxW(nullptr, i18n.ErrGetProcessTokenShutdown.c_str(), i18n.ErrTitle.c_str(),
                    MB_ICONERROR);
        return;
    }
    if (!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid)) {
        MessageBoxW(nullptr, i18n.ErrLookupPrivilegeShutdown.c_str(), i18n.ErrTitle.c_str(),
                    MB_ICONERROR);
        CloseHandle(hToken);
        return;
    }
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, static_cast<PTOKEN_PRIVILEGES>(nullptr), 0);
    CloseHandle(hToken);

    wchar_t msg[] = L"Shutdown...";
    if (!InitiateSystemShutdownEx(NULL, msg, 0, TRUE, FALSE, SHTDN_REASON_MAJOR_OTHER)) {
        MessageBoxW(nullptr, i18n.ErrShutdownFailed.c_str(), i18n.ErrTitle.c_str(), MB_ICONERROR);
    }
}

void ExecuteSleep() {
    auto& i18n = I18N::GetInstance();
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        if (LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid)) {
            tkp.PrivilegeCount = 1;
            tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
            AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, static_cast<PTOKEN_PRIVILEGES>(nullptr),
                                  0);
        }
        CloseHandle(hToken);
    }

    if (!SetSuspendState(FALSE, FALSE, FALSE)) {
        MessageBoxW(nullptr, i18n.ErrSleepFailed.c_str(), i18n.ErrTitle.c_str(), MB_ICONERROR);
    }
}

void ExecuteLock() {
    LockWorkStation();
}

void ExecuteAction(HWND hWnd, Action action) {
    switch (action) {
        case Action::Sleep:
            ExecuteSleep();
            break;
        case Action::Shutdown:
            ExecuteShutdown();
            break;
        case Action::Restart:
            ExecuteRestart();
            break;
        case Action::Lock:
            ExecuteLock();
            break;
        case Action::None:
        default:
            break;
    }

    // Destroy the window will trigger quit message
    DestroyWindow(hWnd);
    return;
}

void StartCountdown(HWND hWnd, Action action) {
    auto& appState = AppState::GetInstance();
    // todo 理论上这里的逻辑和倒计时到期一样
    if (appState.config.delay <= 0) {
        ExecuteAction(hWnd, action);
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

void TriggerLock(HWND hWnd) {
    ExecuteLock();
}

void TriggerConfig(HWND hWnd) {
    auto& appState = AppState::GetInstance();
    std::wstring configPath = appState.config.GetConfigPath();
    ShellExecuteW(NULL, L"open", configPath.c_str(), NULL, NULL, SW_SHOWNORMAL);
    if (appState.windowPage.fading) {
        return;
    }
    appState.windowPage.fading = true;
    SetTimer(hWnd, FADEOUT_TIMER_ID, FRAME_TIME, NULL);
}

void TriggerDonate(HWND hWnd) {
    auto& appState = AppState::GetInstance();
}
