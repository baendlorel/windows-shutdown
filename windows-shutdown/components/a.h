// clickable link component header
#pragma once
#include "div.h"

class A : public Div {
   public:
    std::string url;
    std::wstring text;

   public:
    A(const Gdiplus::RectF& rect, std::string url, std::wstring text);

   public:
    void Draw(const Gdiplus::Graphics* graphics, DrawContext& ctx);
};
