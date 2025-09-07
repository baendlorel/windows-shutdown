#include "consts/button-style.h"
#include "image-button.h"
#include "i18n.h"

ImageButton::ImageButton(int resId, Action action) {
    this->x = 0;
    this->y = 0;
    this->png = nullptr;
    this->resId = resId;
    this->action = action;
}

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
    Gdiplus::Bitmap* image = Gdiplus::Bitmap::FromStream(pStream);
    pStream->Release();
    if (!image) {
        MessageBoxW(nullptr, i18n.ErrCreateImageBitmap.c_str(), i18n.ErrTitle.c_str(),
                    MB_ICONERROR);
        return;
    }
    this->png = image;
}

void ImageButton::Center(int marginLeft, int marginTop, int w, int h) {
    int centerX = w / 2;
    int centerY = h / 2;
    this->x = centerX + marginLeft;
    this->y = centerY + marginTop;
}

bool ImageButton::MouseHit(int mx, int my) const {
    // Account for the same margins used when rendering the button so that
    // hit testing uses the visual center rather than the stored center.
    int dx = mx - this->x;
    int dy = my - this->y;
    return (dx * dx + dy * dy <= BUTTON_TRUE_RADIUS_SQUARED);
}