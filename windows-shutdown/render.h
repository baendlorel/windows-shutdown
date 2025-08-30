#pragma once


static void DrawToMemoryDC(HDC hdcMem, int w, int h, BYTE alpha);

static void DrawTextWithOutline(Graphics& graphics, const wchar_t* text,
                                const Gdiplus::Font& font,
                                const PointF& position, int outlineWidth);

static void UpdateLayered(HWND hWnd, BYTE alpha);
