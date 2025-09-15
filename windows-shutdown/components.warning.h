#pragma once
#include "framework.h"

void draw_warning(Gdiplus::Graphics& graphics, BYTE alpha, Gdiplus::REAL w, Gdiplus::REAL h,
                  const std::wstring& text);
