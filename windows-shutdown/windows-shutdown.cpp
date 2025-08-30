#include "windows-shutdown.h"
#include "config-parser.h"

// order below must be preserved
#include <objidl.h>
#include <gdiplus.h>
#include <windows.h>
#include <string>

#include "framework.h"
#pragma comment(lib, "Gdiplus.lib")

using namespace Gdiplus;


constexpr int MAX_LOADSTRING = 100;
constexpr int FADEIN_DURATION = 150;  // ms
constexpr int FADEIN_INTERVAL = 10;   // ms
constexpr int FADEIN_TIMER_ID = 1001;
constexpr int FADEOUT_TIMER_ID = 1002;
constexpr int COUNTDOWN_TIMER_ID = 1003;  // New timer for countdown

// Function declarations in this module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static ATOM MyRegisterClass(HINSTANCE hInstance);
static BOOL InitInstance(HINSTANCE, int);
static void DrawTextWithOutline(Graphics& graphics, const wchar_t* text, const Gdiplus::Font& font, 
                                const PointF& position, int outlineWidth);

struct ImageButton {
  int x, y, r;
  const wchar_t* label;
  int resId;
};
ImageButton buttons[2] = {{0, 0, 60, L"Restart", IDB_RESTARTPNG},
                          {0, 0, 60, L"Shutdown", IDB_SHUTDOWNPNG}};
ULONG_PTR gdiplusToken;
HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];
int screenW = 0, screenH = 0;
BYTE g_alpha = 0;  // Current alpha
bool g_fadingOut = false;
int hoveredIndex = -1;  // -1 means no button is hovered

// configs
WindowsShutdownConfig config;

// Countdown state
bool isCountingDown = false;
int countdownSeconds = 0;
bool isRestartCountdown = false;  // true for restart, false for shutdown


static ATOM MyRegisterClass(HINSTANCE hInstance) {
  WNDCLASSEXW wcex{};
  wcex.cbSize = sizeof(WNDCLASSEX);
  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = WndProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hInstance;
  wcex.hIcon = NULL;
  wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wcex.hbrBackground = NULL;
  wcex.lpszMenuName = NULL;
  wcex.lpszClassName = szWindowClass;
  wcex.hIconSm = NULL;
  return RegisterClassExW(&wcex);
}

static BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
  hInst = hInstance;
  screenW = GetSystemMetrics(SM_CXSCREEN);
  screenH = GetSystemMetrics(SM_CYSCREEN);
  HWND hWnd =
      CreateWindowExW(WS_EX_LAYERED, szWindowClass, szTitle, WS_POPUP, 0, 0,
                      screenW, screenH, nullptr, nullptr, hInstance, nullptr);
  if (!hWnd) return FALSE;
  ShowWindow(hWnd, SW_SHOW);
  UpdateWindow(hWnd);
  g_alpha = 0;
  SetTimer(hWnd, FADEIN_TIMER_ID, FADEIN_INTERVAL, NULL);
  return TRUE;
}

static Bitmap* LoadPngFromResource(HINSTANCE hInst, int resId) {
  HRSRC hRes = FindResource(hInst, MAKEINTRESOURCE(resId), L"PNG");
  if (!hRes) return nullptr;
  HGLOBAL hMem = LoadResource(hInst, hRes);
  if (!hMem) return nullptr;
  void* pData = LockResource(hMem);
  DWORD size = SizeofResource(hInst, hRes);
  IStream* pStream = nullptr;
  (void)CreateStreamOnHGlobal(NULL, TRUE, &pStream);
  ULONG written;
  pStream->Write(pData, size, &written);
  LARGE_INTEGER li = {0};
  pStream->Seek(li, STREAM_SEEK_SET, NULL);
  Bitmap* bmp = Bitmap::FromStream(pStream);
  pStream->Release();
  return bmp;
}


static void ExecuteRestart() {
  HANDLE hToken;
  TOKEN_PRIVILEGES tkp;
  OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
  LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
  tkp.PrivilegeCount = 1;
  tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
  AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);

  wchar_t msg[] = L"Restarting...";
  //InitiateSystemShutdownEx(NULL, msg, 0, TRUE, TRUE, SHTDN_REASON_MAJOR_OTHER);
}

static void ExecuteShutdown() {
  HANDLE hToken;
  TOKEN_PRIVILEGES tkp;
  OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
  LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
  tkp.PrivilegeCount = 1;
  tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
  AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);

  wchar_t msg[] = L"Shutdown...";
  //InitiateSystemShutdownEx(NULL, msg, 0, TRUE, FALSE, SHTDN_REASON_MAJOR_OTHER);
}

static void StartCountdown(HWND hWnd, bool isRestart) {
  if (config.delay <= 0) {
    // No delay, execute immediately
    if (isRestart) {
      ExecuteRestart();
    } else {
      ExecuteShutdown();
    }
    return;
  }

  isCountingDown = true;
  countdownSeconds = config.delay;
  isRestartCountdown = isRestart;
  SetTimer(hWnd, COUNTDOWN_TIMER_ID, 1000, NULL);  // 1 second interval
  UpdateLayered(hWnd, g_alpha);  // Redraw to show countdown
}

static void CancelCountdown(HWND hWnd) {
  if (isCountingDown) {
    isCountingDown = false;
    KillTimer(hWnd, COUNTDOWN_TIMER_ID);
    UpdateLayered(hWnd, g_alpha);  // Redraw to hide countdown
  }
}

static void TriggerRestart(HWND hWnd) {
  StartCountdown(hWnd, true);
}

static void TriggerShutdown(HWND hWnd) {
  StartCountdown(hWnd, false);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                         LPARAM lParam) {
  switch (message) {
    case WM_TIMER:
      if (wParam == FADEIN_TIMER_ID) {
        BYTE targetAlpha = 255;
        int steps = FADEIN_DURATION / FADEIN_INTERVAL;
        BYTE step = (targetAlpha + steps - 1) / steps;
        if (g_alpha < targetAlpha) {
          g_alpha =
              (g_alpha + step > targetAlpha) ? targetAlpha : g_alpha + step;
          UpdateLayered(hWnd, g_alpha);
        } else {
          KillTimer(hWnd, FADEIN_TIMER_ID);
        }
      } else if (wParam == FADEOUT_TIMER_ID) {
        int steps = FADEIN_DURATION / FADEIN_INTERVAL;
        BYTE step = (255 + steps - 1) / steps;
        if (g_alpha > 0) {
          g_alpha = (g_alpha < step) ? 0 : g_alpha - step;
          UpdateLayered(hWnd, g_alpha);
        } else {
          KillTimer(hWnd, FADEOUT_TIMER_ID);
          DestroyWindow(hWnd);
        }
      } else if (wParam == COUNTDOWN_TIMER_ID) {
        if (isCountingDown) {
          countdownSeconds--;
          if (countdownSeconds <= 0) {
            CancelCountdown(hWnd);
            if (isRestartCountdown) {
              ExecuteRestart();
            } else {
              ExecuteShutdown();
            }
          } else {
            UpdateLayered(hWnd, g_alpha);  // Redraw to update countdown
          }
        }
      }
      break;
    case WM_PAINT: {
      UpdateLayered(hWnd, g_alpha);
    } break;
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
      if (isCountingDown) {
        CancelCountdown(hWnd);
      }
      break;
    case WM_MOUSEMOVE: {
      if (isCountingDown) {
        break;  // Ignore mouse move during countdown
      }
      int mx = LOWORD(lParam);
      int my = HIWORD(lParam);
      int newHover = -1;
      for (int i = 0; i < 2; ++i) {
        int dx = mx - buttons[i].x;
        int dy = my - buttons[i].y;
        if (dx * dx + dy * dy <= buttons[i].r * buttons[i].r) {
          newHover = i;
          break;
        }
      }
      if (newHover != hoveredIndex) {
        hoveredIndex = newHover;
        UpdateLayered(hWnd, g_alpha);
      }
    } break;
    case WM_LBUTTONDOWN: {
      if (isCountingDown) {
        CancelCountdown(hWnd);
        break;
      }
      
      int mx = LOWORD(lParam);
      int my = HIWORD(lParam);
      bool hit = false;
      for (int i = 0; i < 2; ++i) {
        int dx = mx - buttons[i].x;
        int dy = my - buttons[i].y;
        if (dx * dx + dy * dy <= buttons[i].r * buttons[i].r) {
          hit = true;
          if (i == 0) {
            TriggerRestart(hWnd);
          } else {
            TriggerShutdown(hWnd);
          }
          break;
        }
      }
      if (!hit && !g_fadingOut) {
        g_fadingOut = true;
        SetTimer(hWnd, FADEOUT_TIMER_ID, FADEIN_INTERVAL, NULL);
      }
    } break;
    case WM_CLOSE:
      if (isCountingDown) {
        CancelCountdown(hWnd);
      }
      if (!g_fadingOut) {
        g_fadingOut = true;
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

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);
  GdiplusStartupInput gdiplusStartupInput;
  GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

  ParseConfigFile(config);

  if (config.mode == Mode::IMMEDIATE) {
    ExecuteShutdown();  // Execute immediately without UI
    GdiplusShutdown(gdiplusToken);
    return 0;
  }

  LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
  LoadStringW(hInstance, IDC_WINDOWSSHUTDOWN, szWindowClass, MAX_LOADSTRING);
  MyRegisterClass(hInstance);
  if (!InitInstance(hInstance, nCmdShow)) {
    return FALSE;
  }
  MSG msg;
  while (GetMessage(&msg, nullptr, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  GdiplusShutdown(gdiplusToken);
  return (int)msg.wParam;
}
