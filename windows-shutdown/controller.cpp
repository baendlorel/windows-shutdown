#include "controller.h"

#include <powrprof.h>
#include <shellapi.h>

#pragma comment(lib, "PowrProf.lib")

void controller::execute_restart() {
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        MessageBoxW(nullptr, app::i18n.ErrGetProcessTokenRestart.c_str(),
                    app::i18n.ErrTitle.c_str(), MB_ICONERROR);
        return;
    }
    if (!LookupPrivilegeValue(nullptr, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid)) {
        MessageBoxW(nullptr, app::i18n.ErrLookupPrivilegeRestart.c_str(),
                    app::i18n.ErrTitle.c_str(), MB_ICONERROR);
        CloseHandle(hToken);
        return;
    }
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, static_cast<PTOKEN_PRIVILEGES>(nullptr), nullptr);
    CloseHandle(hToken);

    wchar_t msg[] = L"Restarting...";
    if (!InitiateSystemShutdownEx(nullptr, msg, 0, TRUE, TRUE, SHTDN_REASON_MAJOR_OTHER)) {
        MessageBoxW(nullptr, app::i18n.ErrRestartFailed.c_str(), app::i18n.ErrTitle.c_str(),
                    MB_ICONERROR);
    }
}

void controller::execute_shutdown() {
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        MessageBoxW(nullptr, app::i18n.ErrGetProcessTokenShutdown.c_str(),
                    app::i18n.ErrTitle.c_str(), MB_ICONERROR);
        return;
    }
    if (!LookupPrivilegeValue(nullptr, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid)) {
        MessageBoxW(nullptr, app::i18n.ErrLookupPrivilegeShutdown.c_str(),
                    app::i18n.ErrTitle.c_str(), MB_ICONERROR);
        CloseHandle(hToken);
        return;
    }
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, static_cast<PTOKEN_PRIVILEGES>(nullptr), nullptr);
    CloseHandle(hToken);

    wchar_t msg[] = L"Shutdown...";
    if (!InitiateSystemShutdownEx(nullptr, msg, 0, TRUE, FALSE, SHTDN_REASON_MAJOR_OTHER)) {
        MessageBoxW(nullptr, app::i18n.ErrShutdownFailed.c_str(), app::i18n.ErrTitle.c_str(),
                    MB_ICONERROR);
    }
}

void controller::execute_sleep() {
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
        MessageBoxW(nullptr, app::i18n.ErrSleepFailed.c_str(), app::i18n.ErrTitle.c_str(),
                    MB_ICONERROR);
    }
}

void controller::execute_lock() {
    LockWorkStation();
}

void controller::execute_action(const HWND hWnd, const app::Action action) {
    switch (action) {
        case app::Action::Sleep:
            execute_sleep();
            break;
        case app::Action::Shutdown:
            execute_shutdown();
            break;
        case app::Action::Restart:
            execute_restart();
            break;
        case app::Action::Lock:
            execute_lock();
            break;
        case app::Action::Donate:
        case app::Action::Config:
        case app::Action::None:
            break;
    }

    // Destroy the window will trigger quit message
    DestroyWindow(hWnd);
}

// Jump to countdown page and start countdown timer
void controller::start_countdown(const HWND hWnd, const app::Action action) {
    if (app::config.delay <= 0) {
        execute_action(hWnd, action);
        return;
    }

    SetTimer(hWnd, app::COUNTDOWN_TIMER_ID, 1000, nullptr);  // 1 second interval
    app::state.countdown_seconds = app::config.delay;
    app::state.action = action;
    app::page.start(app::Page::Countdown, hWnd);
}

void controller::cancel_countdown(const HWND hWnd) {
    if (app::page.current == app::Page::Countdown) {
        // & do this in page.SetAlpha: appState.action = Action::None;
        KillTimer(hWnd, app::COUNTDOWN_TIMER_ID);

        app::state.countdown_seconds = 0;
        app::page.start(app::Page::Home, hWnd);
        app::page.on_fade_end([]() { app::state.action = app::Action::None; });
    }
}

void controller::trigger_lock(const HWND hWnd) {
    execute_lock();
}

void controller::trigger_config(const HWND hWnd) {
    const std::wstring configPath = AppConfig::get_config_path();
    ShellExecuteW(nullptr, L"open", configPath.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
    if (app::page.fading) {
        return;
    }

    app::page.start(app::Page::None, hWnd);
}

void controller::trigger_donate(const HWND hWnd) {
    app::page.start(app::Page::Donate, hWnd);
}
