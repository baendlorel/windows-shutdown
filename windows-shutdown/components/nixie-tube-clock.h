#pragma once
#include <windows.h>
#include <gdiplus.h>

// Draw nixie tube clock with HH:MM:SS format
// Uses IDB_NIXIE_PERIOD for colons, IDB_NIXIE_BLANK for leading zeros in hours/minutes
// anchor: normalized anchor point (0..1) that specifies which point of the clock
// will be placed at rect.X/Y when rect width/height is zero. Default is center (0.5,0.5).
void DrawNixieTubeClock(Gdiplus::Graphics& graphics, BYTE alpha, Gdiplus::RectF rect,
                        Gdiplus::PointF anchor, int seconds);
