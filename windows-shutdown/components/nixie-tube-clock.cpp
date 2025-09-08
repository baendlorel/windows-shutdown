#include "nixie-tube-clock.h"
#include "resource.h"
#include "bitmap-loader.h"
#include "app-state.h"

// Return pointer to array of bitmaps
Gdiplus::Bitmap** LoadNixieBitmap() {
    HINSTANCE hInst = AppState::GetInstance().hInst;
    // Allocate on heap to avoid returning pointer to stack memory
    Gdiplus::Bitmap** nixieBitmaps = new Gdiplus::Bitmap*[12];  // 0-9, blank, period
    nixieBitmaps[0] = LoadBitmapByResourceId(hInst, IDB_NIXIE_0);
    nixieBitmaps[1] = LoadBitmapByResourceId(hInst, IDB_NIXIE_1);
    nixieBitmaps[2] = LoadBitmapByResourceId(hInst, IDB_NIXIE_2);
    nixieBitmaps[3] = LoadBitmapByResourceId(hInst, IDB_NIXIE_3);
    nixieBitmaps[4] = LoadBitmapByResourceId(hInst, IDB_NIXIE_4);
    nixieBitmaps[5] = LoadBitmapByResourceId(hInst, IDB_NIXIE_5);
    nixieBitmaps[6] = LoadBitmapByResourceId(hInst, IDB_NIXIE_6);
    nixieBitmaps[7] = LoadBitmapByResourceId(hInst, IDB_NIXIE_7);
    nixieBitmaps[8] = LoadBitmapByResourceId(hInst, IDB_NIXIE_8);
    nixieBitmaps[9] = LoadBitmapByResourceId(hInst, IDB_NIXIE_9);
    nixieBitmaps[10] = LoadBitmapByResourceId(hInst, IDB_NIXIE_BLANK);   // blank
    nixieBitmaps[11] = LoadBitmapByResourceId(hInst, IDB_NIXIE_PERIOD);  // period/colon
    return nixieBitmaps;
};

// Draw nixie tube clock with HH:MM:SS format
void DrawNixieTubeClock(Gdiplus::Graphics& graphics, BYTE alpha, Gdiplus::RectF rect, int seconds) {
    if (alpha == 0) {
        return;
    }

    // Load all nixie tube bitmaps only once (static initialization)
    static Gdiplus::Bitmap** nixieBitmaps = LoadNixieBitmap();

    // Convert seconds to HH:MM:SS
    int totalSeconds = seconds;
    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int secs = totalSeconds % 60;

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

    // Calculate individual digit size and positions
    float totalWidth = rect.Width;
    float totalHeight = rect.Height;
    float digitWidth = totalWidth / 8.0f;  // 8 positions total
    float digitHeight = totalHeight;

    // Set up alpha blending
    Gdiplus::ColorMatrix colorMatrix = {
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,           1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, alpha / 255.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

    Gdiplus::ImageAttributes imageAttributes;
    imageAttributes.SetColorMatrix(&colorMatrix);

    // Draw each digit/colon
    for (int i = 0; i < 8; i++) {
        Gdiplus::Bitmap* bitmap = nixieBitmaps[digitIndices[i]];
        if (bitmap) {
            Gdiplus::RectF digitRect(rect.X + i * digitWidth, rect.Y, digitWidth, digitHeight);

            graphics.DrawImage(bitmap, digitRect, 0, 0, static_cast<float>(bitmap->GetWidth()),
                               static_cast<float>(bitmap->GetHeight()), Gdiplus::UnitPixel,
                               &imageAttributes);
        }
    }
}
