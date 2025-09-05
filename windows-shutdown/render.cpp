#include "render.h"
#include <unordered_map>

#include "app-state.h"
#include "i18n.h"

// fixme instruction全部消失了——是alignCenter的问题！

void DrawToMemoryDC(HDC hdcMem, int w, int h) {
    static auto& appState = AppState::GetInstance();
    static auto& i18n = I18N::GetInstance();
    static auto& colors = ColorSet::GetInstance();

    static Gdi::FontFamily fontFamily(i18n.FontFamilyName.c_str());
    static Gdi::SolidBrush bgBrush(colors.BackgroundColor);

    Gdi::Graphics graphics(hdcMem);
    graphics.SetSmoothingMode(Gdi::SmoothingModeAntiAlias);
    graphics.SetTextRenderingHint(Gdi::TextRenderingHintAntiAlias);
    // Draw semi-transparent background
    graphics.FillRectangle(&bgBrush, 0, 0, w, h);

    // Draw countdown in center
    if (appState.isCountingDown()) {
        // First line: original style (left/right language preserved by i18n.Wait)
        std::wstring firstLine = i18n.Wait(appState.action, appState.countdownSeconds);
        // Second line: large centered numeric seconds
        std::wstring secondLine = std::to_wstring(appState.countdownSeconds);

        // Fonts
        Gdi::Font firstFont(&fontFamily, COUNT_DOWN_FONT_SIZE, Gdi::FontStyleRegular);
        // Position first line a bit above center
        Gdi::REAL y = h * 0.36f;
        Gdi::RectF firstRect(0, y, w, h);
        DrawTextParams firstParams = {.text = firstLine,
                                      .font = &firstFont,
                                      .rect = &firstRect,
                                      .horizontalAlign = Gdi::StringAlignmentCenter,
                                      .color = &colors.TextColor,
                                      .shadowColor = &colors.TextShadowColor};
        DrawCachedUIText(graphics, firstParams);

        // Position second (number) centered below the first line
        Gdi::Font secondFont(&fontFamily, COUNT_DOWN_NUMBER_FONT_SIZE, Gdi::FontStyleBold);
        Gdi::RectF secondRect(0, y + 60, w, h);
        DrawTextParams secondParams = {.text = secondLine,
                                       .font = &secondFont,
                                       .rect = &secondRect,
                                       .horizontalAlign = Gdi::StringAlignmentCenter,
                                       .color = &colors.TextColor,
                                       .shadowColor = &colors.TextShadowColor};
        // & seconds cannot use cache since it changes every second
        DrawUIText(graphics, secondParams);

        // Draw cancel instruction below the number
        Gdi::Font smallFont(&fontFamily, INSTRUCTION_FONT_SIZE, Gdi::FontStyleRegular);
        Gdi::RectF smallRect(0, y + 160, w, h);
        DrawTextParams smallParams = {.text = i18n.PressAnyKeyToCancel,
                                      .font = &smallFont,
                                      .rect = &smallRect,
                                      .horizontalAlign = Gdi::StringAlignmentCenter,
                                      .color = &colors.TextColor,
                                      .shadowColor = &colors.TextShadowColor};
        DrawCachedUIText(graphics, smallParams);
        return;
    }

    // Not counting down
    auto& warnings = appState.config.warnings;
    if (!warnings.empty()) {
        Gdi::Font warnFont(&fontFamily, INSTRUCTION_FONT_SIZE, Gdi::FontStyleRegular);
        Gdi::RectF warnRect(CFG_WARNING_X, CFG_WARNING_Y, w, h);
        DrawTextParams warnParams = {.text = i18n.GetConfigWarnings(warnings),
                                     .font = &warnFont,
                                     .rect = &warnRect,
                                     .horizontalAlign = Gdi::StringAlignmentNear,
                                     //  .color = &colors.TextWarnColor,
                                     .color = &colors.TextDangerColor,
                                     .shadowColor = &colors.TextShadowColor};
        DrawCachedUIText(graphics, warnParams);
    }

    // Draw image buttons (original logic)
    for (int i = 0; i < BUTTON_COUNT; ++i) {
        int x = appState.buttons[i].x - BUTTON_RADIUS;
        int y = appState.buttons[i].y - BUTTON_RADIUS;

        graphics.DrawImage(appState.buttons[i].png, x, y, BUTTON_DIAMETER, BUTTON_DIAMETER);
        // If hovered, overlay a semi-transparent white
        if (i == appState.hoveredIndex) {
            Gdi::SolidBrush highlightBrush(colors.ButtonHighlightColor);
            graphics.FillEllipse(&highlightBrush, x + BUTTON_SHADOW_WIDTH, y + BUTTON_SHADOW_WIDTH,
                                 BUTTON_DIAMETER - BUTTON_SHADOW_WIDTH * 2,
                                 BUTTON_DIAMETER - BUTTON_SHADOW_WIDTH * 2);
        }
    }

    // Draw instruction text below buttons
    Gdi::Font font(&fontFamily, INSTRUCTION_FONT_SIZE, Gdi::FontStyleRegular);
    // Below buttons with some margin
    int instrY = (h / 2) + BUTTON_RADIUS + BUTTON_MARGIN_TOP + BUTTON_MARGIN_BOTTOM;

    // Draw text with beautiful rendering
    Gdi::RectF instrRect(0, instrY, w, h);

    DrawTextParams instrParams = {.text = i18n.PressAnyKeyToExit,
                                  .font = &font,
                                  .rect = &instrRect,
                                  .horizontalAlign = Gdi::StringAlignmentCenter,
                                  .color = &colors.TextColor,
                                  .shadowColor = &colors.TextShadowColor};
    DrawCachedUIText(graphics, instrParams);
}

void DrawUITextShadow(Gdi::Graphics& graphics, DrawTextParams& params) {
    Gdi::StringFormat format;
    format.SetAlignment(params.horizontalAlign);
    format.SetLineAlignment(Gdi::StringAlignmentNear);
    Gdi::SolidBrush brush(Gdi::Color(0, 0, 0, 0));
    for (int radius = TEXT_SHADOW_RADIUS; radius >= 1; radius -= TEXT_SHADOW_RADIUS_STEP) {
        int alpha = 80 / (radius + 1);
        brush.SetColor(Gdi::Color(alpha, params.shadowColor->GetR(), params.shadowColor->GetG(),
                                  params.shadowColor->GetB()));
        for (int i = 0; i < 8; i++) {
            float dx = SHADOW_OFFSET[i][0] * radius;
            float dy = SHADOW_OFFSET[i][1] * radius;
            Gdi::RectF rect(params.rect->X + dx, params.rect->Y + dy, params.rect->Width,
                            params.rect->Height);
            graphics.DrawString(params.text.c_str(), -1, params.font, rect, &format, &brush);
        }
    }
}

void DrawUIText(Gdi::Graphics& graphics, DrawTextParams& params) {
    graphics.SetTextRenderingHint(Gdi::TextRenderingHintAntiAliasGridFit);
    graphics.SetSmoothingMode(Gdi::SmoothingModeAntiAlias);
    graphics.SetCompositingQuality(Gdi::CompositingQualityHighQuality);

    DrawUITextShadow(graphics, params);

    Gdi::SolidBrush brush(*params.color);
    Gdi::StringFormat format;
    // & Better to be handled in DrawCachedUIText
    // format.SetAlignment(params.horizontalAlign);
    format.SetAlignment(Gdi::StringAlignmentNear);
    format.SetLineAlignment(Gdi::StringAlignmentNear);
    graphics.DrawString(params.text.c_str(), -1, params.font, *params.rect, &format, &brush);
}

Gdi::Bitmap* UITextToBitmap(Gdi::Graphics& graphics, DrawTextParams& params) {
    // only wstring can be correctly hashed, while LPCWSTR cannot
    static std::unordered_map<std::wstring, Gdi::Bitmap*> cache;
    auto it = cache.find(params.text);
    if (it != cache.end()) {
        return it->second;
    }

    // Measure text size to determine bitmap dimensions
    Gdi::StringFormat format;
    format.SetAlignment(params.horizontalAlign);
    format.SetLineAlignment(Gdi::StringAlignmentNear);

    Gdi::RectF measuredRect(0, 0, params.rect->Width, params.rect->Height);
    Gdi::RectF box;
    graphics.MeasureString(params.text.c_str(), -1, params.font, measuredRect, &format, &box);

    // Add extra margin for shadow (max radius is 4, need extra space in each direction)
    int bitmapWidth = static_cast<int>(box.Width) + TEXT_SHADOW_RADIUS * 2;
    int bitmapHeight = static_cast<int>(box.Height) + TEXT_SHADOW_RADIUS * 2;

    // Create new bitmap
    Gdi::Bitmap* bitmap = new Gdi::Bitmap(bitmapWidth, bitmapHeight, PixelFormat32bppARGB);
    Gdi::Graphics bitmapGraphics(bitmap);

    // Set high quality rendering
    bitmapGraphics.SetTextRenderingHint(Gdi::TextRenderingHintAntiAliasGridFit);
    bitmapGraphics.SetSmoothingMode(Gdi::SmoothingModeAntiAlias);
    bitmapGraphics.SetCompositingQuality(Gdi::CompositingQualityHighQuality);

    // Clear bitmap background to transparent
    bitmapGraphics.Clear(Gdi::Color(0, 0, 0, 0));

    params.rect = &measuredRect;
    DrawUIText(bitmapGraphics, params);

    // Store in cache
    cache[params.text] = bitmap;

    return bitmap;
}

void DrawCachedUIText(Gdi::Graphics& graphics, DrawTextParams& params) {
    Gdi::RectF* rect = params.rect;
    Gdi::Bitmap* cachedBitmap = UITextToBitmap(graphics, params);
    if (!cachedBitmap) {
        return;
    }
    // Calculate draw position (consider shadow margin)
    Gdi::REAL drawX = rect->X - TEXT_SHADOW_RADIUS;
    Gdi::REAL drawY = rect->Y - TEXT_SHADOW_RADIUS;

    // Adjust X position according to alignment
    if (params.horizontalAlign == Gdi::StringAlignmentCenter) {
        drawX = rect->X + rect->Width / 2;
    } else if (params.horizontalAlign == Gdi::StringAlignmentFar) {
        drawX = rect->X + rect->Width + TEXT_SHADOW_RADIUS;
    }

    graphics.DrawImage(cachedBitmap, drawX, drawY);
}

struct WH {
    int w;
    int h;
};

WH GetWH(HWND hWnd, AppState& appState) {
    RECT rc;
    GetClientRect(hWnd, &rc);
    int w = rc.right - rc.left;
    int h = rc.bottom - rc.top;
    for (short i = 0; i < 5; i++) {
        appState.buttons[i].Center(BUTTON_MARGIN_LEFT, BUTTON_MARGIN_TOP, w, h, i);
    }
    return {w, h};
}

void UpdateLayered(HWND hWnd) {
    static auto& appState = AppState::GetInstance();
    static WH wh = GetWH(hWnd, appState);
    HDC hdcScreen = GetDC(NULL);
    HDC hdcMem = CreateCompatibleDC(hdcScreen);
    // BITMAPINFO bmi = {0};
    BITMAPINFO bmi;
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = wh.w;
    bmi.bmiHeader.biHeight = -wh.h;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    void* pvBits = nullptr;
    HBITMAP hBitmap = CreateDIBSection(hdcScreen, &bmi, DIB_RGB_COLORS, &pvBits, NULL, 0);
    if (hBitmap == NULL) {
        static auto& i18n = I18N::GetInstance();
        MessageBoxW(nullptr, i18n.ErrCreateBitmap.c_str(), i18n.ErrTitle.c_str(), MB_ICONERROR);
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);
        return;
    }

    HGDIOBJ oldBmp = SelectObject(hdcMem, hBitmap);
    DrawToMemoryDC(hdcMem, wh.w, wh.h);
    POINT ptWin = {0, 0};
    SIZE sizeWin = {wh.w, wh.h};
    BLENDFUNCTION blend = {AC_SRC_OVER, 0, appState.g_alpha, AC_SRC_ALPHA};
    UpdateLayeredWindow(hWnd, hdcScreen, &ptWin, &sizeWin, hdcMem, &ptWin, 0, &blend, ULW_ALPHA);
    SelectObject(hdcMem, oldBmp);
    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
    ReleaseDC(NULL, hdcScreen);
}