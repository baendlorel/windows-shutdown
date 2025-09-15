#include "framework.h"
#include "consts.app.h"
#include "app.core.h"

#include "window.h"

namespace {
void EnsureDpiAwareness() {
    if (const HMODULE hShcore = LoadLibraryW(L"shcore.dll")) {
        typedef HRESULT(WINAPI * setter)(int /*PROCESS_DPI_AWARENESS*/);

        const auto pSet =
            reinterpret_cast /* NOLINT(clang-diagnostic-cast-function-type-strict) */<setter>(
                GetProcAddress(hShcore, "SetProcessDpiAwareness"));

        if (pSet) {
            // 2 == PROCESS_PER_MONITOR_DPI_AWARE
            (void)pSet(2);
            FreeLibrary(hShcore);
            return;
        }
        FreeLibrary(hShcore);
    }

    // Fallback to old API SetProcessDPIAware (user32.dll)
    if (const HMODULE hUser = LoadLibraryW(L"user32.dll")) {
        typedef BOOL(WINAPI * setter)();
        auto* t = GetProcAddress(hUser, "SetProcessDPIAware");
        if (const auto pOld =
                reinterpret_cast /* NOLINT(clang-diagnostic-cast-function-type-strict) */<setter>(
                    t)) {
            pOld();
        }
        FreeLibrary(hUser);
    }
}
}  // namespace

int APIENTRY wWinMain(_In_ const HINSTANCE hInstance, _In_opt_ const HINSTANCE hPrevInstance,
                      _In_ const LPWSTR lpCmdLine, _In_ const int nShowCmd) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    EnsureDpiAwareness();

    app::config.load();
    app::i18n.set_lang(app::config.lang);
    app::state.hInst = hInstance;

    const Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;

    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    LoadStringW(hInstance, IDS_APP_TITLE, app::state.szTitle, app::MAX_LOAD_STRING);
    LoadStringW(hInstance, IDC_WINDOWSSHUTDOWN, app::state.szWindowClass, app::MAX_LOAD_STRING);

    window::my_register_class();

    // fixme no-no无响应退出了
    if (!window::init_instance(nShowCmd)) {
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
