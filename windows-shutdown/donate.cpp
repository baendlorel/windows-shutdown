#include "donate.h"

#include "style.fade.h"
#include "style.font.h"
#include "style.color.h"

#include "mini-ui.h"

#include "bitmap-loader.h"

void DonateView::DrawView(Gdiplus::Graphics& graphics, int w, int h) {
    BYTE alpha = AppPage::GetInstance().GetPageAlpha(Page::Donate);
    if (alpha == 0) {
        return;
    }

    static auto& colors = ColorSet::GetInstance();
    static Gdiplus::FontFamily fontFamily(app.i18n.FontFamilyName.c_str());
    static Gdiplus::Bitmap* img =
        LoadBitmapByResourceId(app.state.hInst, app.i18n.DonateQRResourceID);

    // localized polite & cute thank-you line from i18n
    // std::wstring thankText = std::format(L"{}\n {}", i18n.Author, i18n.DonateThank);
    constexpr int lineHeight = 80;
    constexpr int marginTop = 100;

    // Draw image centered
    int imgW = img->GetWidth();
    int imgH = img->GetHeight();
    int drawX = (w - imgW) / 2;
    int drawY = (h - imgH) / 2 + marginTop;

    auto imgAttr = ImageAttrWithAlpha(img, alpha);
    Gdiplus::RectF imgRect(drawX, drawY, imgW, imgH);
    graphics.DrawImage(img, imgRect, 0, 0, imgW, imgH, Gdiplus::UnitPixel, imgAttr.get());

    // Draw about text above image
    Gdiplus::Font font(&fontFamily, INSTRUCTION_FONT_SIZE, Gdiplus::FontStyleBold);

    Gdiplus::RectF line1(0, drawY - lineHeight * 3, w, h);
    DrawTextParams line1Params = {.text = app.i18n.Author,
                                  .font = &font,
                                  .rect = &line1,
                                  .horizontalAlign = Gdiplus::StringAlignmentCenter,
                                  .alpha = alpha,
                                  .color = &colors.TextColor,
                                  .shadowColor = &colors.TextShadowColor};
    DrawCachedUIText(graphics, line1Params);

    Gdiplus::RectF line2(0, drawY - lineHeight * 2, w, h);
    DrawTextParams line2Params = {.text = app.i18n.AuthorInfo,
                                  .font = &font,
                                  .rect = &line2,
                                  .horizontalAlign = Gdiplus::StringAlignmentCenter,
                                  .alpha = alpha,
                                  .color = &colors.TextColor,
                                  .shadowColor = &colors.TextShadowColor};
    DrawCachedUIText(graphics, line2Params);

    Gdiplus::RectF line3(0, drawY - lineHeight * 1, w, h);
    DrawTextParams line3Params = {.text = app.i18n.DonateThank,
                                  .font = &font,
                                  .rect = &line3,
                                  .horizontalAlign = Gdiplus::StringAlignmentCenter,
                                  .alpha = alpha,
                                  .color = &colors.TextColor,
                                  .shadowColor = &colors.TextShadowColor};
    DrawCachedUIText(graphics, line3Params);
}
