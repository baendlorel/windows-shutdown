#include "controller.h"

#include "app-state.h"
#include "consts.h"
#include "framework.h"
#include "render.h"
#include "config.h"
#include <powrprof.h>
#include <shellapi.h>

#pragma comment(lib, "PowrProf.lib")
// 声明GetConfigPath函数（来自config.cpp）
std::wstring GetConfigPath();

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
  // 获取必要的权限
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
  
  // 执行睡眠操作
  SetSuspendState(FALSE, FALSE, FALSE);
}

void ExecuteLock() {
  // 锁定工作站
  LockWorkStation();
}

void StartCountdown(HWND hWnd, bool isRestart, bool isSleep) {
  auto& appState = AppState::getInstance();
  if (appState.config.delay <= 0) {
    // No delay, execute immediately
    if (isSleep) {
      // For sleep, close the program first then sleep
      PostMessage(hWnd, WM_CLOSE, 0, 0);
      // Use a small delay to ensure the program closes before sleeping
      Sleep(500);
      ExecuteSleep();
    } else if (isRestart) {
      ExecuteRestart();
    } else {
      ExecuteShutdown();
    }
    return;
  }

  appState.isCountingDown = true;
  appState.countdownSeconds = appState.config.delay;
  appState.isRestartCountdown = isRestart;
  appState.isSleepCountdown = isSleep;
  SetTimer(hWnd, COUNTDOWN_TIMER_ID, 1000, NULL);  // 1 second interval
  UpdateLayered(hWnd, appState.g_alpha);           // Redraw to show countdown
}

void CancelCountdown(HWND hWnd) {
  auto& appState = AppState::getInstance();
  if (appState.isCountingDown) {
    appState.isCountingDown = false;
    appState.isRestartCountdown = false;
    appState.isSleepCountdown = false;
    KillTimer(hWnd, COUNTDOWN_TIMER_ID);
    UpdateLayered(hWnd, appState.g_alpha);  // Redraw to hide countdown
  }
}

void TriggerRestart(HWND hWnd) { StartCountdown(hWnd, true, false); }

void TriggerShutdown(HWND hWnd) { StartCountdown(hWnd, false, false); }

void TriggerSleep(HWND hWnd) { 
  StartCountdown(hWnd, false, true); 
}

void TriggerLock(HWND hWnd) {
  auto& appState = AppState::getInstance();
  if (!appState.g_fadingOut) {
    appState.g_fadingOut = true;
    SetTimer(hWnd, FADEOUT_TIMER_ID, FADEIN_INTERVAL, NULL);
  }
  ExecuteLock();
}

void TriggerConfig(HWND hWnd) {
  auto& appState = AppState::getInstance();
  
  std::wstring configPath = GetConfigPath();
  
  ShellExecuteW(NULL, L"open", configPath.c_str(), NULL, NULL, SW_SHOWNORMAL);
  
  if (!appState.g_fadingOut) {
    appState.g_fadingOut = true;
    SetTimer(hWnd, FADEOUT_TIMER_ID, FADEIN_INTERVAL, NULL);
  }
}