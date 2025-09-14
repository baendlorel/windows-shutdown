#include "framework.h"
#include "consts.app.h"
#include "style.fade.h"
#include "app.core.h"

#include "controller.h"
#include "window.h"

//  _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
// Try modern DPI APIs first (SetProcessDpiAwareness in shcore.dll), fall back to
// SetProcessDPIAware in user32.dll when not available. This uses runtime loading
// to remain compatible with different SDKs/toolchains.
static void EnsureDpiAwareness() {
    // Try SetProcessDpiAwareness (Windows 8.1+, shcore.dll)
    HMODULE hShcore = LoadLibraryW(L"shcore.dll");
    if (hShcore) {
        typedef HRESULT(WINAPI * SetProcessDpiAwareness_t)(int /*PROCESS_DPI_AWARENESS*/);
        SetProcessDpiAwareness_t pSet = reinterpret_cast<SetProcessDpiAwareness_t>(
            GetProcAddress(hShcore, "SetProcessDpiAwareness"));
        if (pSet) {
            // 2 == PROCESS_PER_MONITOR_DPI_AWARE
            pSet(2);
            FreeLibrary(hShcore);
            return;
        }
        FreeLibrary(hShcore);
    }

    // Fallback to old API SetProcessDPIAware (user32.dll)
    HMODULE hUser = LoadLibraryW(L"user32.dll");
    if (hUser) {
        typedef BOOL(WINAPI * SetProcessDPIAware_t)(void);
        SetProcessDPIAware_t pOld =
            reinterpret_cast<SetProcessDPIAware_t>(GetProcAddress(hUser, "SetProcessDPIAware"));
        if (pOld) {
            pOld();
        }
        FreeLibrary(hUser);
    }
}

// msvc use: int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    EnsureDpiAwareness();

    auto& app = App::GetInstance();
    app.config.Load();
    app.i18n.SetLang(app.config.lang);
    app.state.hInst = hInstance;

    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;

    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    LoadStringW(hInstance, IDS_APP_TITLE, app.state.szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSSHUTDOWN, app.state.szWindowClass, MAX_LOADSTRING);

    auto& window = Window::GetInstance();
    window.MyRegisterClass();

    // fixme nono无响应退出了
    if (!window.InitInstance(nCmdShow)) {
        return FALSE;
    }

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    Gdiplus::GdiplusShutdown(gdiplusToken);
    return static_cast<int>(msg.wParam);
}
