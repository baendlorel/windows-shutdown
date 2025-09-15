#include "donate.h"

#include "style.fade.h"
#include "style.font.h"
#include "style.color.h"
#include "realify.h"

#include "mini-ui.h"

#include "bitmap-loader.h"

void DonateView::draw_view(Gdiplus::Graphics& graphics, const DrawParams& params) {
    // TODO 这里要改成统一的isinvisible
    BYTE alpha = AppPage::GetInstance().get_page_alpha(app::Page::Donate);
    if (!params.rect) {
        throw "rect为空";
    }

    int w = params.rect->Width;
    int h = params.rect->Height;

    static auto& colors = color_set::GetInstance();
    static Gdiplus::FontFamily fontFamily(app.i18n.FontFamilyName.c_str());
    static Gdiplus::Bitmap* img =
        load_bitmap_by_resource_id(app.state.hInst, app.i18n.DonateQRResourceID);

    // localized polite & cute thank-you line from i18n
    constexpr int lineHeight = 80;
    constexpr int marginTop = 100;

    // Draw image centered
    Gdiplus::REAL imgW = to_real(img->GetWidth());
    Gdiplus::REAL imgH = to_real(img->GetHeight());
    Gdiplus::REAL drawX = (w - imgW) / 2;
    Gdiplus::REAL drawY = (h - imgH) / 2 + marginTop;

    auto imgAttr = image_attr_with_alpha(img, alpha);
    Gdiplus::RectF imgRect(drawX, drawY, imgW, imgH);
    graphics.DrawImage(img, imgRect, 0, 0, imgW, imgH, Gdiplus::UnitPixel, imgAttr.get());

    // Draw about text above image
    Gdiplus::Font font(&fontFamily, INSTRUCTION_FONT_SIZE, Gdiplus::FontStyleBold);

    Gdiplus::RectF line1(0, drawY - lineHeight * 3, w, h);
    DrawTextParams line1Params = {.text = app.i18n.Author,
                                  .font = &font,
                                  .rect = &line1,
                                  .horizontal_align = Gdiplus::StringAlignmentCenter,
                                  .alpha = alpha,
                                  .color = &colors.text_light,
                                  .shadow_color = &colors.text_shadow};
    DrawCachedUIText(graphics, line1Params);

    Gdiplus::RectF line2(0, drawY - lineHeight * 2, w, h);
    DrawTextParams line2Params = {.text = app.i18n.AuthorInfo,
                                  .font = &font,
                                  .rect = &line2,
                                  .horizontal_align = Gdiplus::StringAlignmentCenter,
                                  .alpha = alpha,
                                  .color = &colors.text_light,
                                  .shadow_color = &colors.text_shadow};
    DrawCachedUIText(graphics, line2Params);

    Gdiplus::RectF line3(0, drawY - lineHeight * 1, w, h);
    DrawTextParams line3Params = {.text = app.i18n.DonateThank,
                                  .font = &font,
                                  .rect = &line3,
                                  .horizontal_align = Gdiplus::StringAlignmentCenter,
                                  .alpha = alpha,
                                  .color = &colors.text_light,
                                  .shadow_color = &colors.text_shadow};
    DrawCachedUIText(graphics, line3Params);
}
