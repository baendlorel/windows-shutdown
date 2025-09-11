// clickable link component header
#pragma once
#include "framework.h"

class A {
   public:
    // Draws a clickable link area
    // graphics: drawing context
    // rect: clickable area
    // url: link to open
    void Draw(Gdiplus::Graphics* graphics, const Gdiplus::RectF& rect, const std::wstring& url);

    // Handles mouse click, returns true if clicked and opened
    bool OnClick(const POINT& pt, const RECT& rect, const std::wstring& url);

    bool MouseHit(int mx, int my) const;

   private:
    Gdiplus::RectF rect;
};
