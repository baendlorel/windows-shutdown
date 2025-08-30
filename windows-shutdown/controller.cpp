
#include "framework.h"
#include "controller.h"
#include "consts.h"
#include "render.h"
#include "app-state.h"

auto appState = AppState::getInstance();

void ExecuteRestart() {
  HANDLE hToken;
  TOKEN_PRIVILEGES tkp;
  OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
                   &hToken);
  LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
  tkp.PrivilegeCount = 1;
  tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
  AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);

  wchar_t msg[] = L"Restarting...";
  // InitiateSystemShutdownEx(NULL, msg, 0, TRUE, TRUE,
  // SHTDN_REASON_MAJOR_OTHER);
}

void ExecuteShutdown() {
  HANDLE hToken;
  TOKEN_PRIVILEGES tkp;
  OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
                   &hToken);
  LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
  tkp.PrivilegeCount = 1;
  tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
  AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);

  wchar_t msg[] = L"Shutdown...";
  // InitiateSystemShutdownEx(NULL, msg, 0, TRUE, FALSE,
  // SHTDN_REASON_MAJOR_OTHER);
}

void StartCountdown(HWND hWnd, bool isRestart) {
  if (appState.config.delay <= 0) {
    // No delay, execute immediately
    if (isRestart) {
      ExecuteRestart();
    } else {
      ExecuteShutdown();
    }
    return;
  }

  appState.isCountingDown = true;
  appState.countdownSeconds = appState.config.delay;
  appState.isRestartCountdown = isRestart;
  SetTimer(hWnd, COUNTDOWN_TIMER_ID, 1000, NULL);  // 1 second interval
  UpdateLayered(hWnd, appState.g_alpha);           // Redraw to show countdown
}

void CancelCountdown(HWND hWnd) {
  if (appState.isCountingDown) {
    appState.isCountingDown = false;
    KillTimer(hWnd, COUNTDOWN_TIMER_ID);
    UpdateLayered(hWnd, appState.g_alpha);  // Redraw to hide countdown
  }
}

void TriggerRestart(HWND hWnd) { StartCountdown(hWnd, true); }

void TriggerShutdown(HWND hWnd) { StartCountdown(hWnd, false); }