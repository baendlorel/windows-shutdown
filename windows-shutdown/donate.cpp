#include "donate.h"

#include <stdexcept>

#include "style.font.h"
#include "style.color.h"
#include "realify.h"
#include "bitmap-loader.h"

#include "mini-ui.h"

void DonateView::draw_view(Gdiplus::Graphics& graphics, const DrawParams& params) {
    // TODO 这里要改成统一的is_invisible
    const BYTE alpha = AppPage::GetInstance().get_page_alpha(app::Page::Donate);
    if (!params.rect) {
        throw std::runtime_error("rect为空");
    }

    const Gdiplus::REAL w = params.rect->Width;
    const Gdiplus::REAL h = params.rect->Height;

    static Gdiplus::FontFamily fontFamily(app::i18n.FontFamilyName.c_str());
    static Gdiplus::Bitmap* img =
        load_bitmap_by_resource_id(app::state.hInst, app::i18n.DonateQRResourceID);

    // localized polite & cute thank-you line from i18n
    constexpr int lineHeight = 80;
    constexpr int marginTop = 100;

    // Draw image centered
    const Gdiplus::REAL imgW = to_real(img->GetWidth());
    const Gdiplus::REAL imgH = to_real(img->GetHeight());
    const Gdiplus::REAL drawX = (w - imgW) / 2;
    const Gdiplus::REAL drawY = (h - imgH) / 2 + marginTop;

    const auto imgAttr = painter::image_attr_with_alpha(img, alpha);
    const Gdiplus::RectF imgRect(drawX, drawY, imgW, imgH);
    graphics.DrawImage(img, imgRect, 0, 0, imgW, imgH, Gdiplus::UnitPixel, imgAttr.get());

    // Draw about text above image
    Gdiplus::Font font(&fontFamily, font_style::INSTRUCTION_FONT_SIZE, Gdiplus::FontStyleBold);

    Gdiplus::RectF line1(0, drawY - lineHeight * 3, w, h);
    painter::draw_cached_text(graphics, {.text = app::i18n.Author,
                                         .font = &font,
                                         .rect = &line1,
                                         .horizontal_align = Gdiplus::StringAlignmentCenter,
                                         .alpha = alpha,
                                         .color = &color_set::TEXT_LIGHT,
                                         .shadow_color = &color_set::TEXT_SHADOW});

    Gdiplus::RectF line2(0, drawY - lineHeight * 2, w, h);
    painter::draw_cached_text(graphics, {.text = app::i18n.AuthorInfo,
                                         .font = &font,
                                         .rect = &line2,
                                         .horizontal_align = Gdiplus::StringAlignmentCenter,
                                         .alpha = alpha,
                                         .color = &color_set::TEXT_LIGHT,
                                         .shadow_color = &color_set::TEXT_SHADOW});

    Gdiplus::RectF line3(0, drawY - lineHeight * 1, w, h);
    painter::draw_cached_text(graphics, {.text = app::i18n.DonateThank,
                                         .font = &font,
                                         .rect = &line3,
                                         .horizontal_align = Gdiplus::StringAlignmentCenter,
                                         .alpha = alpha,
                                         .color = &color_set::TEXT_LIGHT,
                                         .shadow_color = &color_set::TEXT_SHADOW});
}
