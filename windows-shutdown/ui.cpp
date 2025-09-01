#include "ui.h"

#include "Resource.h"
#include "app-state.h"
#include "consts.h"
#include "framework.h"

void CenterButtons(int w, int h) {
    auto& appState = AppState::getInstance();
    int centerX = w / 2;
    int centerY = h / 2;
    int spacing = BUTTON_GAP + BUTTON_RADIUS;

    for (short i = 0; i < 5; i++) {
        appState.buttons[i].x = centerX + spacing * (i - 2);
        appState.buttons[i].y = centerY;
    }
}

Bitmap* LoadPngFromResource(HINSTANCE hInst, int resId) {
    HRSRC hRes = FindResource(hInst, MAKEINTRESOURCE(resId), L"PNG");
    if (!hRes) {
        return nullptr;
    }
    HGLOBAL hMem = LoadResource(hInst, hRes);
    if (!hMem) {
        return nullptr;
    }

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