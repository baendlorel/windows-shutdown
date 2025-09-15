#pragma once
#include "framework.h"
#include "utils.class.h"
#include "app.core.h"

#include "index.h"

class Render {
    SINGLETON(Render)

   public:
    Index* index = Index::GetInstancePtr();

    void draw_to_memory_dc(const HDC hdcMem, const Gdiplus::REAL w, const Gdiplus::REAL h);
    void update_layered(const HWND hWnd);

   private:
    void __DrawDebug(Gdiplus::Graphics& graphics, Gdiplus::REAL w, Gdiplus::REAL h);
    SIZE get_wh(const HWND hWnd) const;
};
