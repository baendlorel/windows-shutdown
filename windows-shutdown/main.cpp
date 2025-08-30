#include "windows-shutdown.h"
#include "consts.h"
#include "framework.h"
#include "app-state.h"
#include "window.h"
#include "controller.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  AppState::getInstance().hInst = hInstance;
  
  GdiplusStartupInput gdiplusStartupInput;
  ULONG_PTR gdiplusToken;

  GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

  if (AppState::getInstance().config.mode == Mode::IMMEDIATE) {
    ExecuteShutdown();  // Execute immediately without UI
    GdiplusShutdown(gdiplusToken);
    return 0;
  }

  LoadStringW(hInstance, IDS_APP_TITLE, AppState::getInstance().szTitle,
              MAX_LOADSTRING);
  LoadStringW(hInstance, IDC_WINDOWSSHUTDOWN,
              AppState::getInstance().szWindowClass,
              MAX_LOADSTRING);
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
