#include "mini-ui.h"
#include <unordered_map>
#include <format>
#include "style.font.h"
#include "dtm.h"

Gdiplus::Color ApplyAlpha(Gdiplus::Color* color, BYTE alpha) {
    if (alpha == FADE::MAX_ALPHA) {
        return *color;
    }

    // overallAlpha in 0..255, srcA in 0..255
    int blended = (alpha * color->GetA()) / 255;
    if (blended < 0) {
        blended = 0;
    } else if (blended > 255) {
        blended = 255;
    }
    return Gdiplus::Color(blended, color->GetR(), color->GetG(), color->GetB());
};

std::unique_ptr<Gdiplus::ImageAttributes> ImageAttrWithAlpha(Gdiplus::Image* image, BYTE alpha) {
    if (alpha == FADE::MAX_ALPHA) {
        return nullptr;
    }

    Gdiplus::ColorMatrix colorMatrix = {
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,           1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, alpha / 255.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
    auto imgAttr = std::make_unique<Gdiplus::ImageAttributes>();
    imgAttr->SetColorMatrix(&colorMatrix);
    return imgAttr;
}

void DrawUITextShadow(Gdiplus::Graphics& graphics, DrawTextParams& params) {
    Gdiplus::StringFormat format;
    auto align = params.manualAlign ? Gdiplus::StringAlignmentNear : params.horizontalAlign;
    // & Better to be handled in DrawCachedUIText, here we always use near
    format.SetAlignment(align);
    format.SetLineAlignment(Gdiplus::StringAlignmentNear);
    Gdiplus::SolidBrush brush(Gdiplus::Color(0, 0, 0, 0));

    // temporarily change compositing mode to SourceCopy to avoid blending multiple shadows
    for (int radius = TEXT_SHADOW_RADIUS; radius >= 1; radius -= TEXT_SHADOW_RADIUS_STEP) {
        BYTE alpha = (TEXT_SHADOW_ALPHA * params.alpha) / ((radius + 1) * FADE::MAX_ALPHA);
        brush.SetColor(ApplyAlpha(params.shadowColor, alpha));
        for (int i = 0; i < 16; i++) {
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

    Gdiplus::SolidBrush brush(ApplyAlpha(params.color, params.alpha));
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

    // temporarily set alpha to 255 to get correct cached bitmap
    BYTE alpha = params.alpha;
    params.alpha = FADE::MAX_ALPHA;
    Gdiplus::Bitmap* img = UITextToBitmap(graphics, params);
    if (!img) {
        return;
    }
    // Calculate draw position (consider shadow margin)
    Gdiplus::REAL drawX = rect->X;
    Gdiplus::REAL drawY = rect->Y;

    // Adjust X position according to alignment
    if (params.manualAlign) {
        if (params.horizontalAlign == Gdiplus::StringAlignmentCenter) {
            drawX += (rect->Width - img->GetWidth()) / 2 + TEXT_SHADOW_RADIUS;
        } else if (params.horizontalAlign == Gdiplus::StringAlignmentFar) {
            drawX += rect->Width - img->GetWidth() + TEXT_SHADOW_RADIUS;
        } else if (params.horizontalAlign == Gdiplus::StringAlignmentNear) {
            drawX += -TEXT_SHADOW_RADIUS;
        }
    }

    if (alpha == FADE::MAX_ALPHA) {
        graphics.DrawImage(img, drawX, drawY);
    } else {
        auto imgAttr = ImageAttrWithAlpha(img, alpha);
        Gdiplus::REAL imgW = static_cast<Gdiplus::REAL>(img->GetWidth());
        Gdiplus::REAL imgH = static_cast<Gdiplus::REAL>(img->GetHeight());
        Gdiplus::RectF rect(drawX, drawY, imgW, imgH);
        graphics.DrawImage(img, rect, 0, 0, rect.Width, rect.Height, Gdiplus::UnitPixel,
                           imgAttr.get());
    }
}
