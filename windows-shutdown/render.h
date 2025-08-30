#pragma once
#include "framework.h"

void DrawToMemoryDC(HDC hdcMem, int w, int h, BYTE alpha);

void DrawTextWithOutline(Graphics& graphics, const wchar_t* text,
                                const Gdiplus::Font& font,
                                const PointF& position, int outlineWidth);

void UpdateLayered(HWND hWnd, BYTE alpha);
