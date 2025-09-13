#include "bitmap-loader.h"
#include "app.core.h"

Gdiplus::Bitmap* LoadBitmapByResourceId(HINSTANCE hInst, int resId) {
    auto& i18n = App::GetInstance().i18n;
    HRSRC hRes = FindResource(hInst, MAKEINTRESOURCE(resId), L"PNG");
    if (!hRes) {
        MessageBoxW(nullptr, i18n.ErrResourceNotFound.c_str(), i18n.ErrTitle.c_str(), MB_ICONERROR);
        return nullptr;
    }
    HGLOBAL hMem = LoadResource(hInst, hRes);
    if (!hMem) {
        MessageBoxW(nullptr, i18n.ErrLoadResource.c_str(), i18n.ErrTitle.c_str(), MB_ICONERROR);
        return nullptr;
    }

    void* pData = LockResource(hMem);
    DWORD size = SizeofResource(hInst, hRes);
    IStream* pStream = nullptr;
    if (FAILED(CreateStreamOnHGlobal(NULL, TRUE, &pStream))) {
        MessageBoxW(nullptr, i18n.ErrCreateImageStream.c_str(), i18n.ErrTitle.c_str(),
                    MB_ICONERROR);
        return nullptr;
    }
    ULONG written;
    pStream->Write(pData, size, &written);
    // LARGE_INTEGER li = {0};
    LARGE_INTEGER li = {0};
    pStream->Seek(li, STREAM_SEEK_SET, NULL);
    Gdiplus::Bitmap* image = Gdiplus::Bitmap::FromStream(pStream);
    pStream->Release();
    if (!image) {
        MessageBoxW(nullptr, i18n.ErrCreateImageBitmap.c_str(), i18n.ErrTitle.c_str(),
                    MB_ICONERROR);
        return nullptr;
    }
    return image;
}
