#include "windows-shutdown.h"
#include "config-parser.h"

// order below must be preserved
#include <objidl.h>
#include <gdiplus.h>
#include <windows.h>

#include "framework.h"
#pragma comment(lib, "Gdiplus.lib")

using namespace Gdiplus;


constexpr int MAX_LOADSTRING = 100;
constexpr int FADEIN_DURATION = 150;  // ms
constexpr int FADEIN_INTERVAL = 10;   // ms
constexpr int FADEIN_TIMER_ID = 1001;
constexpr int FADEOUT_TIMER_ID = 1002;

// Function declarations in this module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static ATOM MyRegisterClass(HINSTANCE hInstance);
static BOOL InitInstance(HINSTANCE, int);

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

static void CenterButtons(int w, int h) {
  int centerX = w / 2;
  int centerY = h / 2;
  int spacing = 100;
  int r = 60;
  buttons[0] = {centerX - spacing, centerY, r, L"Restart", IDB_RESTARTPNG};
  buttons[1] = {centerX + spacing, centerY, r, L"Shutdown", IDB_SHUTDOWNPNG};
}

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

static void DrawToMemoryDC(HDC hdcMem, int w, int h, BYTE alpha) {
  Graphics graphics(hdcMem);
  graphics.SetSmoothingMode(SmoothingModeAntiAlias);
  // Draw semi-transparent white background
  SolidBrush bgBrush(Color(77, 255, 255, 255));
  graphics.FillRectangle(&bgBrush, 0, 0, w, h);
  // Draw image buttons
  for (int i = 0; i < 2; ++i) {
    Bitmap* bmp = LoadPngFromResource(hInst, buttons[i].resId);
    if (bmp) {
      int x = buttons[i].x - buttons[i].r;
      int y = buttons[i].y - buttons[i].r;
      int size = buttons[i].r * 2;
      graphics.DrawImage(bmp, x, y, size, size);
      // If hovered, overlay a semi-transparent white
      if (i == hoveredIndex) {
        SolidBrush highlightBrush(Color(28, 255, 255, 255));
        graphics.FillEllipse(&highlightBrush, x, y, size, size);
      }
      delete bmp;
    }
  }
}

static void UpdateLayered(HWND hWnd, BYTE alpha) {
  RECT rc;
  GetClientRect(hWnd, &rc);
  int w = rc.right - rc.left;
  int h = rc.bottom - rc.top;
  CenterButtons(w, h);
  HDC hdcScreen = GetDC(NULL);
  HDC hdcMem = CreateCompatibleDC(hdcScreen);
  BITMAPINFO bmi = {0};
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biWidth = w;
  bmi.bmiHeader.biHeight = -h;
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32;
  bmi.bmiHeader.biCompression = BI_RGB;
  void* pvBits = nullptr;
  HBITMAP hBitmap =
      CreateDIBSection(hdcScreen, &bmi, DIB_RGB_COLORS, &pvBits, NULL, 0);
  if (hBitmap == NULL) {
    DeleteDC(hdcMem);
    ReleaseDC(NULL, hdcScreen);
    return;
  }

  HGDIOBJ oldBmp = SelectObject(hdcMem, hBitmap);
  DrawToMemoryDC(hdcMem, w, h, alpha);
  POINT ptWin = {0, 0};
  SIZE sizeWin = {w, h};
  BLENDFUNCTION blend = {AC_SRC_OVER, 0, alpha, AC_SRC_ALPHA};
  UpdateLayeredWindow(hWnd, hdcScreen, &ptWin, &sizeWin, hdcMem, &ptWin, 0,
                      &blend, ULW_ALPHA);
  SelectObject(hdcMem, oldBmp);
  DeleteObject(hBitmap);
  DeleteDC(hdcMem);
  ReleaseDC(NULL, hdcScreen);
}

static void TriggerRestart() {
  HANDLE hToken;
  TOKEN_PRIVILEGES tkp;
  OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
  LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
  tkp.PrivilegeCount = 1;
  tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
  AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
  //ExitWindowsEx(EWX_REBOOT | EWX_FORCE, SHTDN_REASON_MAJOR_OTHER);

  wchar_t msg[] = L"Restarting...";
  InitiateSystemShutdownEx(NULL,msg, 5, TRUE,
                           TRUE, 
                           SHTDN_REASON_MAJOR_OTHER);
}

static void TriggerShutdown() {
  HANDLE hToken;
  TOKEN_PRIVILEGES tkp;
  OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
  LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
  tkp.PrivilegeCount = 1;
  tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
  AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
  //ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, SHTDN_REASON_MAJOR_OTHER);
  wchar_t msg[] = L"Shutdown...";
  InitiateSystemShutdownEx(NULL,           
                           msg, 
                           5,              
                           TRUE,           
                           FALSE,          
                           SHTDN_REASON_MAJOR_OTHER  
  );
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
      }
      break;
    case WM_PAINT: {
      UpdateLayered(hWnd, g_alpha);
    } break;
    case WM_MOUSEMOVE: {
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
      int mx = LOWORD(lParam);
      int my = HIWORD(lParam);
      bool hit = false;
      for (int i = 0; i < 2; ++i) {
        int dx = mx - buttons[i].x;
        int dy = my - buttons[i].y;
        if (dx * dx + dy * dy <= buttons[i].r * buttons[i].r) {
          hit = true;
          if (i == 0) {
            TriggerRestart();
          } else {
            TriggerShutdown();
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

  WindowsShutdownConfig config = ParseConfigFile();
  if (config.mode == Mode::IMMEDIATE) {
    TriggerShutdown();
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
