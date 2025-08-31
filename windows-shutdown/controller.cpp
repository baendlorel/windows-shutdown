#include "controller.h"

#include "app-state.h"
#include "consts.h"
#include "framework.h"
#include "render.h"
#include <powrprof.h>

#pragma comment(lib, "PowrProf.lib")

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
  InitiateSystemShutdownEx(NULL, msg, 0, TRUE, TRUE, SHTDN_REASON_MAJOR_OTHER);
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
  InitiateSystemShutdownEx(NULL, msg, 0, TRUE, FALSE, SHTDN_REASON_MAJOR_OTHER);
}

void ExecuteSleep() {
  // ��ȡ��Ҫ��Ȩ��
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
  
  // ִ��˯�߲���
  SetSuspendState(FALSE, FALSE, FALSE);
}

void ExecuteLock() {
  // ��������վ
  LockWorkStation();
}

void StartCountdown(HWND hWnd, bool isRestart) {
  auto& appState = AppState::getInstance();
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
  auto& appState = AppState::getInstance();
  if (appState.isCountingDown) {
    appState.isCountingDown = false;
    KillTimer(hWnd, COUNTDOWN_TIMER_ID);
    UpdateLayered(hWnd, appState.g_alpha);  // Redraw to hide countdown
  }
}

void TriggerRestart(HWND hWnd) { StartCountdown(hWnd, true); }

void TriggerShutdown(HWND hWnd) { StartCountdown(hWnd, false); }

void TriggerSleep(HWND hWnd) {
  auto& appState = AppState::getInstance();
  // ˯�߹���ͨ������ִ�У�����Ҫ����ʱ
  if (!appState.g_fadingOut) {
    appState.g_fadingOut = true;
    SetTimer(hWnd, FADEOUT_TIMER_ID, FADEIN_INTERVAL, NULL);
  }
  ExecuteSleep();
}

void TriggerLock(HWND hWnd) {
  auto& appState = AppState::getInstance();
  // ������������ִ��
  if (!appState.g_fadingOut) {
    appState.g_fadingOut = true;
    SetTimer(hWnd, FADEOUT_TIMER_ID, FADEIN_INTERVAL, NULL);
  }
  ExecuteLock();
}

void TriggerConfig(HWND hWnd) {
  auto& appState = AppState::getInstance();
  // Close the current window with fade out

  // TODO: Show configuration dialog or window
  // For now, just close the application
  // You can implement a configuration dialog here later
}