#include "image-button.h"
#include "i18n.h"

void ImageButton::LoadPNGFromResource(HINSTANCE hInst) {
    auto& i18n = I18N::GetInstance();
    HRSRC hRes = FindResource(hInst, MAKEINTRESOURCE(resId), L"PNG");
    if (!hRes) {
        MessageBoxW(nullptr, i18n.ErrResourceNotFound.c_str(), i18n.ErrTitle.c_str(), MB_ICONERROR);
        return;
    }
    HGLOBAL hMem = LoadResource(hInst, hRes);
    if (!hMem) {
        MessageBoxW(nullptr, i18n.ErrLoadResource.c_str(), i18n.ErrTitle.c_str(), MB_ICONERROR);
        return;
    }

    void* pData = LockResource(hMem);
    DWORD size = SizeofResource(hInst, hRes);
    IStream* pStream = nullptr;
    if (FAILED(CreateStreamOnHGlobal(NULL, TRUE, &pStream))) {
        MessageBoxW(nullptr, i18n.ErrCreateImageStream.c_str(), i18n.ErrTitle.c_str(),
                    MB_ICONERROR);
        return;
    }
    ULONG written;
    pStream->Write(pData, size, &written);
    // LARGE_INTEGER li = {0};
    LARGE_INTEGER li;
    pStream->Seek(li, STREAM_SEEK_SET, NULL);
    Bitmap* image = Bitmap::FromStream(pStream);
    pStream->Release();
    if (!image) {
        MessageBoxW(nullptr, i18n.ErrCreateImageBitmap.c_str(), i18n.ErrTitle.c_str(),
                    MB_ICONERROR);
        return;
    }
    this->png = image;
}

void ImageButton::Center(int w, int h, int index) {
    int centerX = w / 2;
    int centerY = h / 2;
    this->x = centerX + BUTTON_CENTER_DISTANCE * (index - 2);
    this->y = centerY;
}
