#include "controller.h"

#include <powrprof.h>
#include <shellapi.h>
#pragma comment(lib, "PowrProf.lib")

#include "consts.app.h"

void Controller::ExecuteRestart() {
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        MessageBoxW(nullptr, app.i18n.ErrGetProcessTokenRestart.c_str(), app.i18n.ErrTitle.c_str(),
                    MB_ICONERROR);
        return;
    }
    if (!LookupPrivilegeValue(nullptr, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid)) {
        MessageBoxW(nullptr, app.i18n.ErrLookupPrivilegeRestart.c_str(), app.i18n.ErrTitle.c_str(),
                    MB_ICONERROR);
        CloseHandle(hToken);
        return;
    }
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, static_cast<PTOKEN_PRIVILEGES>(nullptr), nullptr);
    CloseHandle(hToken);

    wchar_t msg[] = L"Restarting...";
    if (!InitiateSystemShutdownEx(nullptr, msg, 0, TRUE, TRUE, SHTDN_REASON_MAJOR_OTHER)) {
        MessageBoxW(nullptr, app.i18n.ErrRestartFailed.c_str(), app.i18n.ErrTitle.c_str(),
                    MB_ICONERROR);
    }
}

void Controller::ExecuteShutdown() {
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        MessageBoxW(nullptr, app.i18n.ErrGetProcessTokenShutdown.c_str(), app.i18n.ErrTitle.c_str(),
                    MB_ICONERROR);
        return;
    }
    if (!LookupPrivilegeValue(nullptr, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid)) {
        MessageBoxW(nullptr, app.i18n.ErrLookupPrivilegeShutdown.c_str(), app.i18n.ErrTitle.c_str(),
                    MB_ICONERROR);
        CloseHandle(hToken);
        return;
    }
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, static_cast<PTOKEN_PRIVILEGES>(nullptr), nullptr);
    CloseHandle(hToken);

    wchar_t msg[] = L"Shutdown...";
    if (!InitiateSystemShutdownEx(nullptr, msg, 0, TRUE, FALSE, SHTDN_REASON_MAJOR_OTHER)) {
        MessageBoxW(nullptr, app.i18n.ErrShutdownFailed.c_str(), app.i18n.ErrTitle.c_str(),
                    MB_ICONERROR);
    }
}

void Controller::ExecuteSleep() {
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        if (LookupPrivilegeValue(nullptr, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid)) {
            tkp.PrivilegeCount = 1;
            tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
            AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, static_cast<PTOKEN_PRIVILEGES>(nullptr),
                                  nullptr);
        }
        CloseHandle(hToken);
    }

    if (!SetSuspendState(FALSE, FALSE, FALSE)) {
        MessageBoxW(nullptr, app.i18n.ErrSleepFailed.c_str(), app.i18n.ErrTitle.c_str(),
                    MB_ICONERROR);
    }
}

void Controller::ExecuteLock() {
    LockWorkStation();
}

void Controller::ExecuteAction(HWND hWnd, Action action) {
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

// Jump to countdown page and start countdown timer
void Controller::StartCountdown(HWND hWnd, Action action) {
    if (app.config.delay <= 0) {
        ExecuteAction(hWnd, action);
        return;
    }

    SetTimer(hWnd, COUNTDOWN_TIMER_ID, 1000, nullptr);  // 1 second interval
    app.state.countdownSeconds = app.config.delay;
    app.state.action = action;
    app.page.Start(Page::Countdown, hWnd);
}

void Controller::CancelCountdown(HWND hWnd) {
    if (app.page.current == Page::Countdown) {
        // & do this in page.SetAlpha: appState.action = Action::None;
        KillTimer(hWnd, COUNTDOWN_TIMER_ID);

        app.state.countdownSeconds = 0;
        app.page.Start(Page::Home, hWnd);
        app.page.OnFadeEnd([]() { App::GetInstance().state.action = Action::None; });
    }
}

void Controller::TriggerLock(HWND hWnd) {
    ExecuteLock();
}

void Controller::TriggerConfig(HWND hWnd) {
    std::wstring configPath = app.config.get_config_path();
    ShellExecuteW(nullptr, L"open", configPath.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
    if (app.page.fading) {
        return;
    }

    app.page.Start(Page::None, hWnd);
}

void Controller::TriggerDonate(HWND hWnd) {
    app.page.Start(Page::Donate, hWnd);
}
