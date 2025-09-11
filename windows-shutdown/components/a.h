// clickable link component header
#pragma once
#include "div.h"

class A : public Div {
   public:
    std::wstring url;
    std::wstring text;

   public:
    A(const Gdiplus::RectF& rect, std::wstring url, std::wstring text);

   public:
    void Draw(Gdiplus::Graphics& graphics, DrawParams& params);
};
