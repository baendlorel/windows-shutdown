#pragma once
#include "framework.h"
namespace Gdi = Gdiplus;

void DrawToMemoryDC(HDC hdcMem, int w, int h);

void DrawUIText(Gdi::Graphics& graphics, LPCWSTR text, const Gdi::Font& font,
                const Gdi::RectF& rect, Gdi::StringAlignment horizontalAlign,
                const Gdi::Color& textColor, const Gdi::Color& shadowColor);

void DrawUITextShadow(Gdi::Graphics& graphics, LPCWSTR text, const Gdi::Font& font,
                      const Gdi::RectF& rect, Gdi::StringAlignment horizontalAlign,
                      const Gdi::Color& color);

void UpdateLayered(HWND hWnd);
