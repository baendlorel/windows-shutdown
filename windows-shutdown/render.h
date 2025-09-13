#pragma once
#include "framework.h"
#include "singleton.h"
#include "app.core.h"

#include "index.h"

class Render {
    SINGLETON(Render)

   public:
    App& app = App::GetInstance();
    Index& index = Index::GetInstance();

   public:
    void DrawToMemoryDC(HDC hdcMem, Gdiplus::REAL w, Gdiplus::REAL h);
    void UpdateLayered(HWND hWnd);

   private:
    void __DrawDebug(Gdiplus::Graphics& graphics, Gdiplus::REAL w, Gdiplus::REAL h);
    SIZE GetWH(HWND hWnd);
};
