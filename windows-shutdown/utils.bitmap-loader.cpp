#include "utils.bitmap-loader.h"
#include "app.core.h"

Gdiplus::Bitmap* load_bitmap_by_resource_id(const HINSTANCE hInst, const int resId) {
    const HRSRC hRes = FindResource(hInst, MAKEINTRESOURCE(resId), L"PNG");
    if (!hRes) {
        MessageBoxW(nullptr, app::i18n.ErrResourceNotFound.c_str(), app::i18n.ErrTitle.c_str(),
                    MB_ICONERROR);
        return nullptr;
    }
    const HGLOBAL hMem = LoadResource(hInst, hRes);
    if (!hMem) {
        MessageBoxW(nullptr, app::i18n.ErrLoadResource.c_str(), app::i18n.ErrTitle.c_str(),
                    MB_ICONERROR);
        return nullptr;
    }

    const void* pData = LockResource(hMem);
    const DWORD size = SizeofResource(hInst, hRes);
    IStream* pStream = nullptr;
    if (FAILED(CreateStreamOnHGlobal(NULL, TRUE, &pStream))) {
        MessageBoxW(nullptr, app::i18n.ErrCreateImageStream.c_str(), app::i18n.ErrTitle.c_str(),
                    MB_ICONERROR);
        return nullptr;
    }
    ULONG written;
    (void)pStream->Write(pData, size, &written);
    constexpr LARGE_INTEGER li = {};
    (void)pStream->Seek(li, STREAM_SEEK_SET, nullptr);
    Gdiplus::Bitmap* image = Gdiplus::Bitmap::FromStream(pStream);
    pStream->Release();
    if (!image) {
        MessageBoxW(nullptr, app::i18n.ErrCreateImageBitmap.c_str(), app::i18n.ErrTitle.c_str(),
                    MB_ICONERROR);
        return nullptr;
    }
    return image;
}
