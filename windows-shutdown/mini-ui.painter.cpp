#include "mini-ui.h"
#include <unordered_map>
#include <format>
#include <ranges>

#include "style.font.h"
#include "realify.h"

Gdiplus::Color painter::apply_alpha(const Gdiplus::Color* color, const BYTE alpha) {
    if (alpha == fade::MAX_ALPHA) {
        return *color;
    }

    // overallAlpha in 0..255, srcA in 0..255
    BYTE blended = alpha * color->GetA() / fade::MAX_ALPHA;

    return {blended, color->GetR(), color->GetG(), color->GetB()};
};

std::unique_ptr<Gdiplus::ImageAttributes> painter::image_attr_with_alpha(
    const Gdiplus::Image* image, const BYTE alpha) {
    if (alpha == fade::MAX_ALPHA) {
        return nullptr;
    }

    const Gdiplus::ColorMatrix colorMatrix = {{{1.0f, 0.0f, 0.0f, 0.0f, 0.0f},
                                               {0.0f, 1.0f, 0.0f, 0.0f, 0.0f},
                                               {0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
                                               {0.0f, 0.0f, 0.0f, fade::get_opacity(alpha), 0.0f},
                                               {0.0f, 0.0f, 0.0f, 0.0f, 1.0f}}};
    auto imgAttr = std::make_unique<Gdiplus::ImageAttributes>();

    imgAttr->SetColorMatrix(&colorMatrix);
    return imgAttr;
}

void painter::draw_text_shadow(Gdiplus::Graphics& graphics, const DrawTextParams& params) {
    Gdiplus::StringFormat format;
    const auto align = params.manual_align ? Gdiplus::StringAlignmentNear : params.horizontal_align;
    // & Better to be handled in DrawCachedUIText, here we always use near
    format.SetAlignment(align);
    format.SetLineAlignment(Gdiplus::StringAlignmentNear);
    Gdiplus::SolidBrush brush(Gdiplus::Color(0, 0, 0, 0));

    // temporarily change compositing mode to SourceCopy to avoid blending multiple shadows
    for (int radius = font_style::SHADOW_RADIUS; radius >= 1;
         radius -= font_style::SHADOW_RADIUS_STEP) {
        const BYTE alpha =
            font_style::SHADOW_ALPHA * params.alpha / ((radius + 1) * fade::MAX_ALPHA);
        brush.SetColor(apply_alpha(params.shadow_color, alpha));

        for (const int i : std::views::iota(0, 16)) {
            const float dx = font_style::SHADOW_OFFSET[i][0] * to_real(radius);
            const float dy = font_style::SHADOW_OFFSET[i][1] * to_real(radius);
            Gdiplus::RectF rect(params.rect->X + dx, params.rect->Y + dy, params.rect->Width,
                                params.rect->Height);
            graphics.DrawString(params.text.c_str(), -1, params.font, rect, &format, &brush);
        }
    }
}

void painter::draw_text(Gdiplus::Graphics& graphics, const DrawTextParams& params) {
    graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAliasGridFit);
    graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
    graphics.SetCompositingQuality(Gdiplus::CompositingQualityHighQuality);

    draw_text_shadow(graphics, params);

    Gdiplus::SolidBrush brush(apply_alpha(params.color, params.alpha));
    Gdiplus::StringFormat format;
    // & Better to be handled in DrawCachedUIText, here we always use near
    // format.SetAlignment(params.horizontalAlign);
    const auto align = params.manual_align ? Gdiplus::StringAlignmentNear : params.horizontal_align;
    format.SetAlignment(align);
    format.SetLineAlignment(Gdiplus::StringAlignmentNear);
    graphics.DrawString(params.text.c_str(), -1, params.font, *params.rect, &format, &brush);
}

Gdiplus::Bitmap* painter::text_to_bitmap(const Gdiplus::Graphics& graphics,
                                         const DrawTextParams& params) {
    // Cache for rendered UI text bitmaps
    static std::unordered_map<std::wstring, Gdiplus::Bitmap*> cache;

    if (const auto it = cache.find(params.text); it != cache.end()) {
        return it->second;
    }

    // Measure text size to determine bitmap dimensions
    Gdiplus::StringFormat format;
    format.SetAlignment(params.horizontal_align);
    format.SetLineAlignment(Gdiplus::StringAlignmentNear);

    Gdiplus::RectF measuredRect(0, 0, params.rect->Width, params.rect->Height);
    Gdiplus::RectF box;
    graphics.MeasureString(params.text.c_str(), -1, params.font, measuredRect, &format, &box);

    // Add extra margin for shadow (max radius is 4, need extra space in each direction)
    const int bitmapWidth = to_int(box.Width) + font_style::SHADOW_RADIUS * 2;
    const int bitmapHeight = to_int(box.Height) + font_style::SHADOW_RADIUS * 2;

    // Create new bitmap
    Gdiplus::Bitmap* bitmap = new Gdiplus::Bitmap(bitmapWidth, bitmapHeight, PixelFormat32bppARGB);
    Gdiplus::Graphics bitmapGraphics(bitmap);

    // Set high quality rendering
    bitmapGraphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAliasGridFit);
    bitmapGraphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
    bitmapGraphics.SetCompositingQuality(Gdiplus::CompositingQualityHighQuality);

    // Clear bitmap background to transparent
    bitmapGraphics.Clear(Gdiplus::Color(0, 0, 0, 0));

    draw_text(bitmapGraphics, {.text = params.text,
                               .font = params.font,
                               .rect = &measuredRect,
                               .manual_align = params.manual_align,
                               .horizontal_align = params.horizontal_align,
                               .alpha = params.alpha,
                               .color = params.color,
                               .shadow_color = params.shadow_color});

    // Store in cache
    cache[params.text] = bitmap;

    return bitmap;
}

void painter::draw_cached_text(Gdiplus::Graphics& graphics, const DrawTextParams& params) {
    const Gdiplus::RectF* rect = params.rect;

    // temporarily set alpha to 255 to get correct cached bitmap
    BYTE alpha = params.alpha;
    Gdiplus::Bitmap* image =
        painter::text_to_bitmap(graphics, {.text = params.text,
                                           .font = params.font,
                                           .rect = params.rect,
                                           .manual_align = params.manual_align,
                                           .horizontal_align = params.horizontal_align,
                                           .alpha = fade::MAX_ALPHA,
                                           .color = params.color,
                                           .shadow_color = params.shadow_color});
    if (!image) {
        return;
    }
    // Calculate draw position (consider shadow margin)
    Gdiplus::REAL drawX = rect->X;
    const Gdiplus::REAL drawY = rect->Y;
    const Gdiplus::REAL imageW = to_real(image->GetWidth());

    // Adjust X position according to alignment
    if (params.manual_align) {
        if (params.horizontal_align == Gdiplus::StringAlignmentCenter) {
            drawX += (rect->Width - imageW) / 2 + font_style::SHADOW_RADIUS;
        } else if (params.horizontal_align == Gdiplus::StringAlignmentFar) {
            drawX += rect->Width - imageW + font_style::SHADOW_RADIUS;
        } else if (params.horizontal_align == Gdiplus::StringAlignmentNear) {
            drawX += -font_style::SHADOW_RADIUS;
        }
    }

    if (alpha == fade::MAX_ALPHA) {
        graphics.DrawImage(image, drawX, drawY);
    } else {
        const auto imgAttr = image_attr_with_alpha(image, alpha);
        const Gdiplus::REAL imgW = to_real(image->GetWidth());
        const Gdiplus::REAL imgH = to_real(image->GetHeight());
        const Gdiplus::RectF drawRect(drawX, drawY, imgW, imgH);
        graphics.DrawImage(image, drawRect, 0, 0, drawRect.Width, drawRect.Height,
                           Gdiplus::UnitPixel, imgAttr.get());
    }
}
