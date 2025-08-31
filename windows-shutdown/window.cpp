#include "framework.h"
#include "consts.h"
#include "app-state.h"
#include "render.h"
#include "controller.h"
#include "window.h"
#include "Resource.h"


ATOM MyRegisterClass() {
  auto& appState = AppState::getInstance();
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
  auto& appState = AppState::getInstance();
 appState.screenW = GetSystemMetrics(SM_CXSCREEN);
 appState.screenH = GetSystemMetrics(SM_CYSCREEN);
 HWND hWnd = CreateWindowExW(WS_EX_LAYERED, appState.szWindowClass,
      appState.szTitle, WS_POPUP, 0, 0,
     appState.screenW, appState.screenH,
                      nullptr,
                              nullptr, appState.hInst, nullptr);
  if (!hWnd) {
    return FALSE;
  }
  ShowWindow(hWnd, SW_SHOW);
  UpdateWindow(hWnd);
  appState.g_alpha = 0;
  SetTimer(hWnd, FADEIN_TIMER_ID, FADEIN_INTERVAL, NULL);
  return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                         LPARAM lParam) {
  auto& appState = AppState::getInstance();
  switch (message) {
    case WM_TIMER:
      if (wParam == FADEIN_TIMER_ID) {
        BYTE targetAlpha = 255;
        int steps = FADEIN_DURATION / FADEIN_INTERVAL;
        BYTE step = (targetAlpha + steps - 1) / steps;
        if (appState.g_alpha < targetAlpha) {
          appState.g_alpha = (appState.g_alpha + step > targetAlpha)
                                 ? targetAlpha
                                 : appState.g_alpha + step;
          UpdateLayered(hWnd, appState.g_alpha);
        } else {
          KillTimer(hWnd, FADEIN_TIMER_ID);
        }
      } else if (wParam == FADEOUT_TIMER_ID) {
        int steps = FADEIN_DURATION / FADEIN_INTERVAL;
        BYTE step = (255 + steps - 1) / steps;
        if (appState.g_alpha > 0) {
          appState.g_alpha =
              (appState.g_alpha < step) ? 0 : appState.g_alpha - step;
          UpdateLayered(hWnd, appState.g_alpha);
        } else {
          KillTimer(hWnd, FADEOUT_TIMER_ID);
          DestroyWindow(hWnd);
        }
      } else if (wParam == COUNTDOWN_TIMER_ID) {
        if (appState.isCountingDown) {
          appState.countdownSeconds--;
          if (appState.countdownSeconds <= 0) {
            CancelCountdown(hWnd);
            if (appState.isRestartCountdown) {
              ExecuteRestart();
            } else {
              ExecuteShutdown();
            }
          } else {
            // Redraw to update countdown
            UpdateLayered(hWnd, appState.g_alpha);
          }
        }
      }
      break;
    case WM_PAINT: {
      UpdateLayered(hWnd, appState.g_alpha);
    } break;
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
      if (appState.isCountingDown) {
        CancelCountdown(hWnd);
      } else {
        if (!appState.g_fadingOut) {
          appState.g_fadingOut = true;
          SetTimer(hWnd, FADEOUT_TIMER_ID, FADEIN_INTERVAL, NULL);
        }
      }
      break;
    case WM_MOUSEMOVE: {
      if (appState.isCountingDown) {
        break;  // Ignore mouse move during countdown
      }
      int mx = LOWORD(lParam);
      int my = HIWORD(lParam);
      int newHover = -1;
      for (int i = 0; i < 2; ++i) {
        int dx = mx - appState.buttons[i].x;
        int dy = my - appState.buttons[i].y;
        if (dx * dx + dy * dy <=
            appState.buttons[i].r * appState.buttons[i].r) {
          newHover = i;
          break;
        }
      }
      if (newHover != appState.hoveredIndex) {
        appState.hoveredIndex = newHover;
        UpdateLayered(hWnd, appState.g_alpha);
      }
    } break;
    case WM_LBUTTONDOWN: {
      if (appState.isCountingDown) {
        CancelCountdown(hWnd);
        break;
      }

      int mx = LOWORD(lParam);
      int my = HIWORD(lParam);
      bool hit = false;
      
      for (int i = 0; i < 2; ++i) {
        int dx = mx - appState.buttons[i].x;
        int dy = my - appState.buttons[i].y;
        if (dx * dx + dy * dy <=
            appState.buttons[i].r * appState.buttons[i].r) {
          hit = true;
          if (i == 0) {
            TriggerRestart(hWnd);
          } else {
            TriggerShutdown(hWnd);
          }
          break;
        }
      }
      
      if (!hit && !appState.g_fadingOut) {
        appState.g_fadingOut = true;
        SetTimer(hWnd, FADEOUT_TIMER_ID, FADEIN_INTERVAL, NULL);
      }
    } break;
    case WM_CLOSE:
      if (appState.isCountingDown) {
        CancelCountdown(hWnd);
      }
      if (!appState.g_fadingOut) {
        appState.g_fadingOut = true;
        SetTimer(hWnd, FADEOUT_TIMER_ID, FADEIN_INTERVAL, NULL);
      }
      break;
    case WM_DESTROY:
      PostQuitMessage(0);
      break;
    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}