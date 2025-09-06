#include "ui.h"

void DrawUITextShadow(Gdiplus::Graphics& graphics, DrawTextParams& params) {
    Gdiplus::StringFormat format;
    auto align = params.manualAlign ? Gdiplus::StringAlignmentNear : params.horizontalAlign;
    // & Better to be handled in DrawCachedUIText, here we always use near
    format.SetAlignment(align);
    format.SetLineAlignment(Gdiplus::StringAlignmentNear);
    Gdiplus::SolidBrush brush(Gdiplus::Color(0, 0, 0, 0));
    for (int radius = TEXT_SHADOW_RADIUS; radius >= 1; radius -= TEXT_SHADOW_RADIUS_STEP) {
        int alpha = 80 / (radius + 1);
        brush.SetColor(Gdiplus::Color(alpha, params.shadowColor->GetR(), params.shadowColor->GetG(),
                                      params.shadowColor->GetB()));
        for (int i = 0; i < 8; i++) {
            float dx = SHADOW_OFFSET[i][0] * radius;
            float dy = SHADOW_OFFSET[i][1] * radius;
            Gdiplus::RectF rect(params.rect->X + dx, params.rect->Y + dy, params.rect->Width,
                                params.rect->Height);
            graphics.DrawString(params.text.c_str(), -1, params.font, rect, &format, &brush);
        }
    }
}

void DrawUIText(Gdiplus::Graphics& graphics, DrawTextParams& params) {
    graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAliasGridFit);
    graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
    graphics.SetCompositingQuality(Gdiplus::CompositingQualityHighQuality);

    DrawUITextShadow(graphics, params);

    Gdiplus::SolidBrush brush(*params.color);
    Gdiplus::StringFormat format;
    // & Better to be handled in DrawCachedUIText, here we always use near
    // format.SetAlignment(params.horizontalAlign);
    auto align = params.manualAlign ? Gdiplus::StringAlignmentNear : params.horizontalAlign;
    format.SetAlignment(align);
    format.SetLineAlignment(Gdiplus::StringAlignmentNear);
    graphics.DrawString(params.text.c_str(), -1, params.font, *params.rect, &format, &brush);
}

Gdiplus::Bitmap* UITextToBitmap(Gdiplus::Graphics& graphics, DrawTextParams& params) {
    // Cache for rendered UI text bitmaps
    static std::unordered_map<std::wstring, Gdiplus::Bitmap*> cache;

    // only wstring can be correctly hashed, while LPCWSTR cannot
    auto it = cache.find(params.text);
    if (it != cache.end()) {
        return it->second;
    }

    // Measure text size to determine bitmap dimensions
    Gdiplus::StringFormat format;
    format.SetAlignment(params.horizontalAlign);
    format.SetLineAlignment(Gdiplus::StringAlignmentNear);

    Gdiplus::RectF measuredRect(0, 0, params.rect->Width, params.rect->Height);
    Gdiplus::RectF box;
    graphics.MeasureString(params.text.c_str(), -1, params.font, measuredRect, &format, &box);

    // Add extra margin for shadow (max radius is 4, need extra space in each direction)
    int bitmapWidth = static_cast<int>(box.Width) + TEXT_SHADOW_RADIUS * 2;
    int bitmapHeight = static_cast<int>(box.Height) + TEXT_SHADOW_RADIUS * 2;

    // Create new bitmap
    Gdiplus::Bitmap* bitmap = new Gdiplus::Bitmap(bitmapWidth, bitmapHeight, PixelFormat32bppARGB);
    Gdiplus::Graphics bitmapGraphics(bitmap);

    // Set high quality rendering
    bitmapGraphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAliasGridFit);
    bitmapGraphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
    bitmapGraphics.SetCompositingQuality(Gdiplus::CompositingQualityHighQuality);

    // Clear bitmap background to transparent
    bitmapGraphics.Clear(Gdiplus::Color(0, 0, 0, 0));

    params.rect = &measuredRect;
    DrawUIText(bitmapGraphics, params);

    // Store in cache
    cache[params.text] = bitmap;

    return bitmap;
}

void DrawCachedUIText(Gdiplus::Graphics& graphics, DrawTextParams& params) {
    Gdiplus::RectF* rect = params.rect;
    Gdiplus::Bitmap* cachedBitmap = UITextToBitmap(graphics, params);
    if (!cachedBitmap) {
        return;
    }
    // Calculate draw position (consider shadow margin)
    Gdiplus::REAL drawX = rect->X;
    Gdiplus::REAL drawY = rect->Y;

    // Adjust X position according to alignment
    if (params.manualAlign) {
        if (params.horizontalAlign == Gdiplus::StringAlignmentCenter) {
            drawX += (rect->Width - cachedBitmap->GetWidth()) / 2 + TEXT_SHADOW_RADIUS;
        } else if (params.horizontalAlign == Gdiplus::StringAlignmentFar) {
            drawX += rect->Width - cachedBitmap->GetWidth() + TEXT_SHADOW_RADIUS;
        } else if (params.horizontalAlign == Gdiplus::StringAlignmentNear) {
            drawX += -TEXT_SHADOW_RADIUS;
        }
    }

    graphics.DrawImage(cachedBitmap, drawX, drawY);
}
