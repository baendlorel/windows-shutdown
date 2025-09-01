#include "image-button.h"
#include <stdexcept>

void ImageButton::LoadPNGFromResource(HINSTANCE hInst) {
    HRSRC hRes = FindResource(hInst, MAKEINTRESOURCE(resId), L"PNG");
    if (!hRes) {
        throw std::runtime_error("Resource not found: " + std::to_string(resId)); 
    }
    HGLOBAL hMem = LoadResource(hInst, hRes);
    if (!hMem) {
        throw std::runtime_error("Load resource failed: " + std::to_string(resId));
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
    if (!image) {
        throw std::runtime_error("Image from stream failed: " + std::to_string(resId));
    }
    this->png = image;
}

void ImageButton::Center(int w, int h, int index) {
    int centerX = w / 2;
    int centerY = h / 2;
    this->x = centerX + BUTTON_CENTER_DISTANCE * (index - 2);
    this->y = centerY;
}
