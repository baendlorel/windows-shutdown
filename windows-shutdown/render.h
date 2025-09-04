#pragma once
#include "framework.h"

void DrawToMemoryDC(HDC hdcMem, int w, int h);

void DrawUIText(Gdiplus::Graphics& graphics, LPCWSTR text, const Gdiplus::Font& font,
                Gdiplus::REAL width, Gdiplus::REAL y, const Gdiplus::Color& textColor,
                const Gdiplus::Color& shadowColor);

void UpdateLayered(HWND hWnd);
