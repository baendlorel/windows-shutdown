// clickable link component header
#pragma once
#include "framework.h"
#include "div.h"

class A : public Div {
   public:
    void Draw(Gdiplus::Graphics* graphics, const Gdiplus::RectF& rect, const std::wstring& url,
              const std::wstring& text = std::wstring());
};
