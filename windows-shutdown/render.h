#pragma once
#include "framework.h"

void DrawToMemoryDC(HDC hdcMem, int w, int h, BYTE alpha);

void DrawBeautifulText(Graphics& graphics, const wchar_t* text, const Gdiplus::Font& font,
                       REAL width, REAL y, const Color& textColor = Color(255, 255, 255, 255),
                       const Color& shadowColor = Color(128, 0, 0, 0));

void UpdateLayered(HWND hWnd);
