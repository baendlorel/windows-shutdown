#include "Resource.h"
#include "app-state.h"
#include "consts.h"
#include "i18n.h"
#include "controller.h"
#include "framework.h"
#include "window.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    SetProcessDPIAware();

    auto& appState = AppState::GetInstance();
    appState.hInst = hInstance;

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;

    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // must put this after GdiplusStartup, or images will not be loaded correctly
    for (short i = 0; i < BUTTON_COUNT; i++) {
        appState.buttons[i].LoadPNGFromResource(hInstance);
    }

    I18N::Init(appState);

    if (appState.config.isImmediate()) {
        ExecuteShutdown();  // Execute immediately without UI
        GdiplusShutdown(gdiplusToken);
        return 0;
    }

    LoadStringW(hInstance, IDS_APP_TITLE, appState.szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSSHUTDOWN, appState.szWindowClass, MAX_LOADSTRING);
    MyRegisterClass();
    if (!InitInstance(nCmdShow)) {
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
