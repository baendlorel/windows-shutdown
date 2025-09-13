#pragma once
#include "framework.h"
#include "singleton.h"
#include "app.core.h"

#include "index.h"

class Renderer {
    SINGLETON(Renderer)

   public:
    App& app = App::GetInstance();
    Index& index = Index::GetInstance();

   public:
    void DrawToMemoryDC(HDC hdcMem, int w, int h);
    void UpdateLayered(HWND hWnd);

   private:
    void __DrawDebug(Gdiplus::Graphics& graphics, int w, int h);
    SIZE GetWH(HWND hWnd);
};
