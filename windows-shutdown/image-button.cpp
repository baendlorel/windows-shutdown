#include "image-button.h"

void ImageButton::LoadPNGFromResource(HINSTANCE hInst) {
    HRSRC hRes = FindResource(hInst, MAKEINTRESOURCE(resId), L"PNG");
    if (!hRes) {
        throw "PNGResourceNotFound";
    }
    HGLOBAL hMem = LoadResource(hInst, hRes);
    if (!hMem) {
        throw "LoadResourceFailed";
    }

    void* pData = LockResource(hMem);
    DWORD size = SizeofResource(hInst, hRes);
    IStream* pStream = nullptr;
    (void)CreateStreamOnHGlobal(NULL, TRUE, &pStream);
    ULONG written;
    pStream->Write(pData, size, &written);
    LARGE_INTEGER li = {0};
    pStream->Seek(li, STREAM_SEEK_SET, NULL);
    Bitmap* image = Bitmap::FromStream(pStream);
    pStream->Release();
    this->png = image;
}
