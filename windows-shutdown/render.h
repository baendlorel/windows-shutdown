#pragma once
#include "framework.h"
namespace Gdi = Gdiplus;

struct DrawTextParams {
    std::wstring text;
    Gdi::Font* font;
    Gdi::RectF* rect;
    Gdi::StringAlignment horizontalAlign;
    Gdi::Color* color;
    Gdi::Color* shadowColor;

    DrawTextParams GetShadowVersion();
};

void DrawToMemoryDC(HDC hdcMem, int w, int h);

void DrawUIText(Gdi::Graphics& graphics, DrawTextParams& params);

void DrawUITextShadow(Gdi::Graphics& graphics, DrawTextParams& params);

Gdi::Bitmap* UITextToBitmap(Gdi::Graphics& graphics, DrawTextParams& params);

void DrawCachedUIText(Gdi::Graphics& graphics, DrawTextParams& params);

void UpdateLayered(HWND hWnd);
