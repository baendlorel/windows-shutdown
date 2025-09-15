#pragma once
#include "framework.h"
#include "utils.class.h"

#include "index.h"

class Render {
    SINGLETON(Render)

   public:
    Index* index = Index::get_instance_ptr();

    void draw_to_memory_dc(HDC hdcMem, Gdiplus::REAL w, Gdiplus::REAL h) const;
    void update_layered(HWND hWnd);

   private:
    void debug_draw_some_info(Gdiplus::Graphics& graphics, Gdiplus::REAL w, Gdiplus::REAL h) const;
    SIZE get_wh(HWND hWnd) const;
};
