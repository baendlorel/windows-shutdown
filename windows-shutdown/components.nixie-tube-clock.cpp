#include "components.nixie-tube-clock.h"

#include "resource.h"
#include "utils.bitmap-loader.h"
#include "app.core.h"
#include "realify.h"

namespace {
// Return pointer to array of bitmaps
Gdiplus::Bitmap** load_nixie_bitmap() {
    const HINSTANCE hInst = app::state.hInst;
    // Use static array so it's loaded once and no heap management needed
    static Gdiplus::Bitmap* nixieBitmaps[12];  // 0-9, blank, period
    nixieBitmaps[0] = load_bitmap_by_resource_id(hInst, IDB_NIXIE_0);
    nixieBitmaps[1] = load_bitmap_by_resource_id(hInst, IDB_NIXIE_1);
    nixieBitmaps[2] = load_bitmap_by_resource_id(hInst, IDB_NIXIE_2);
    nixieBitmaps[3] = load_bitmap_by_resource_id(hInst, IDB_NIXIE_3);
    nixieBitmaps[4] = load_bitmap_by_resource_id(hInst, IDB_NIXIE_4);
    nixieBitmaps[5] = load_bitmap_by_resource_id(hInst, IDB_NIXIE_5);
    nixieBitmaps[6] = load_bitmap_by_resource_id(hInst, IDB_NIXIE_6);
    nixieBitmaps[7] = load_bitmap_by_resource_id(hInst, IDB_NIXIE_7);
    nixieBitmaps[8] = load_bitmap_by_resource_id(hInst, IDB_NIXIE_8);
    nixieBitmaps[9] = load_bitmap_by_resource_id(hInst, IDB_NIXIE_9);
    nixieBitmaps[10] = load_bitmap_by_resource_id(hInst, IDB_NIXIE_BLANK);   // blank
    nixieBitmaps[11] = load_bitmap_by_resource_id(hInst, IDB_NIXIE_PERIOD);  // period/colon
    return nixieBitmaps;
}
}  // namespace

// Draw nixie tube clock with HH:MM:SS format
void draw_nixie_tube_clock(Gdiplus::Graphics& graphics, const BYTE alpha, const Gdiplus::RectF rect,
                           const Gdiplus::PointF& anchor, const int seconds) {
    if (alpha == 0) {
        return;
    }

    // Load all nixie tube bitmaps only once (static initialization)
    static Gdiplus::Bitmap** nixieBitmaps = load_nixie_bitmap();

    // Convert seconds to HH:MM:SS
    const int totalSeconds = seconds;
    const int hours = totalSeconds / 3600;
    const int minutes = totalSeconds % 3600 / 60;
    const int secs = totalSeconds % 60;

    // Determine which bitmaps to use for each digit
    int digitIndices[8];  // H1 H2 : M1 M2 : S1 S2 format (8 positions)

    // Hours
    if (hours == 0) {
        digitIndices[0] = 10;  // blank
        digitIndices[1] = 10;  // blank
    } else if (hours < 10) {
        digitIndices[0] = 10;  // blank for leading zero
        digitIndices[1] = hours;
    } else {
        digitIndices[0] = hours / 10;
        digitIndices[1] = hours % 10;
    }

    digitIndices[2] = 11;  // colon

    // Minutes
    if (hours == 0 && minutes == 0) {
        digitIndices[3] = 10;  // blank
        digitIndices[4] = 10;  // blank
    } else if (minutes < 10) {
        digitIndices[3] = 0;  // show leading zero for minutes
        digitIndices[4] = minutes;
    } else {
        digitIndices[3] = minutes / 10;
        digitIndices[4] = minutes % 10;
    }

    digitIndices[5] = 11;  // colon

    // Seconds (always show with leading zero if needed)
    if (secs < 10) {
        digitIndices[6] = 0;
        digitIndices[7] = secs;
    } else {
        digitIndices[6] = secs / 10;
        digitIndices[7] = secs % 10;
    }

    // Determine drawing sizes. If rect width/height are zero, use bitmap native sizes
    float totalWidth = rect.Width;
    float totalHeight = rect.Height;
    const bool useNativeSize = (totalWidth == 0.0f || totalHeight == 0.0f);

    float digitWidth;
    float digitHeight;
    if (useNativeSize) {
        // Use first available digit (assume all digits same size) to compute native sizes
        if (Gdiplus::Bitmap* sample = nixieBitmaps[0] ? nixieBitmaps[0] : nixieBitmaps[10]) {
            digitWidth = static_cast<float>(sample->GetWidth());
            digitHeight = static_cast<float>(sample->GetHeight());
        } else {
            // fallback to small defaults
            digitWidth = 20.0f;
            digitHeight = 40.0f;
        }
        totalWidth = digitWidth * 8.0f;
        totalHeight = digitHeight;
    } else {
        digitWidth = totalWidth / 8.0f;  // 8 positions total
        digitHeight = totalHeight;
    }

    // Set up alpha blending (color matrix is 5x5 row-major)
    const Gdiplus::ColorMatrix colorMatrix = {{{1.0f, 0.0f, 0.0f, 0.0f, 0.0f},
                                               {0.0f, 1.0f, 0.0f, 0.0f, 0.0f},
                                               {0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
                                               {0.0f, 0.0f, 0.0f, fade::get_opacity(alpha), 0.0f},
                                               {0.0f, 0.0f, 0.0f, 0.0f, 1.0f}}};

    Gdiplus::ImageAttributes imageAttributes;
    imageAttributes.SetColorMatrix(&colorMatrix, Gdiplus::ColorMatrixFlagsDefault,
                                   Gdiplus::ColorAdjustTypeBitmap);

    // Compute origin position: if using native size and rect was zero-sized, use anchor
    float originX = rect.X;
    float originY = rect.Y;
    if (useNativeSize) {
        // anchor is (0..1) where (0,0)=top-left, (0.5,0.5)=center
        originX = rect.X - anchor.X * totalWidth;
        originY = rect.Y - anchor.Y * totalHeight;
    }

    // Draw each digit/colon
    for (int i = 0; i < 8; i++) {
        Gdiplus::Bitmap* bitmap = nixieBitmaps[digitIndices[i]];
        if (!bitmap) {
            continue;
        }

        Gdiplus::RectF digitRect(originX + to_real(i) * digitWidth, originY, digitWidth,
                                 digitHeight);

        graphics.DrawImage(bitmap, digitRect, 0, 0, to_real(bitmap->GetWidth()),
                           to_real(bitmap->GetHeight()), Gdiplus::UnitPixel, &imageAttributes);
    }
}
