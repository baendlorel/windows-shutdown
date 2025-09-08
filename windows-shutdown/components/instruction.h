#pragma once
#include "../framework.h"

// & Only draw instruction needs internal state check
void DrawInstruction(Gdiplus::Graphics& graphics, BYTE alpha, Gdiplus::RectF* rect,
                     const std::wstring& text);