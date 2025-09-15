#pragma once
#include "framework.h"

void draw_instruction(Gdiplus::Graphics& graphics, BYTE alpha, Gdiplus::RectF* rect,
                      const std::wstring& text);