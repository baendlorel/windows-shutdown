#include "ui.h"

#include "Resource.h"
#include "app-state.h"
#include "consts.h"
#include "framework.h"

void CenterButtons(int w, int h) {
    auto& appState = AppState::getInstance();
    for (short i = 0; i < 5; i++) {
        appState.buttons[i].Center(w, h, i);
    }
}

// todo only once
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