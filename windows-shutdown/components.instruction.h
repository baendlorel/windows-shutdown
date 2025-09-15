#pragma once
#include "framework.h"

void draw_instruction(Gdiplus::Graphics& graphics, const BYTE alpha, Gdiplus::RectF* rect,
                      const std::wstring& text);