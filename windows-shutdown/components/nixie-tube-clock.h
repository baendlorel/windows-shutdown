#pragma once
#include "framework.h"

// Draw nixie tube clock with HH:MM:SS format
// Uses IDB_NIXIE_PERIOD for colons, IDB_NIXIE_BLANK for leading zeros in hours/minutes
void DrawNixieTubeClock(Gdiplus::Graphics& graphics, BYTE alpha, Gdiplus::RectF rect, int seconds);
