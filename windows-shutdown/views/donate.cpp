#include "donate.h"
#include <string>

#include "consts/effects.h"
#include "consts/color-set.h"
#include "consts/font-style.h"
#include "app-state.h"
#include "i18n.h"
#include "bitmap-loader.h"
#include "ui.h"

// Load and cache donate bitmap from assets folder using I18N::DonateQrFileName
static Gdiplus::Bitmap* LoadDonateBitmap() {
    static Gdiplus::Bitmap* img = LoadBitmapByResourceId(AppState::GetInstance().hInst,
                                                         I18N::GetInstance().DonateQRResourceID);
    return img;
}

void DrawDonate(Gdiplus::Graphics& graphics, int w, int h) {
    static auto& i18n = I18N::GetInstance();
    BYTE alpha = AppState::GetInstance().page.GetPageAlpha(Page::Donate);
    if (alpha == 0) {
        return;
    }

    static auto& colors = ColorSet::GetInstance();
    static Gdiplus::FontFamily fontFamily(i18n.FontFamilyName.c_str());
    static Gdiplus::Bitmap* img = LoadDonateBitmap();

    // localized polite & cute thank-you line from i18n
    // std::wstring thankText = std::format(L"{}\n {}", i18n.Author, i18n.DonateThank);

    // Draw image centered
    int imgW = img->GetWidth();
    int imgH = img->GetHeight();
    int drawX = (w - imgW) / 2;
    int drawY = (h - imgH) / 2;

    auto imgAttr = ImageAttrWithAlpha(img, alpha);
    Gdiplus::RectF imgRect(drawX, drawY, imgW, imgH);
    graphics.DrawImage(img, imgRect, 0, 0, imgW, imgH, Gdiplus::UnitPixel, imgAttr.get());

    // Draw about text above image
    Gdiplus::Font font(&fontFamily, INSTRUCTION_FONT_SIZE, Gdiplus::FontStyleBold);

    Gdiplus::RectF line1(0, drawY - 300, w, h);
    DrawTextParams line1Params = {.text = i18n.Author,
                                  .font = &font,
                                  .rect = &line1,
                                  .horizontalAlign = Gdiplus::StringAlignmentCenter,
                                  .alpha = alpha,
                                  .color = &colors.TextColor,
                                  .shadowColor = &colors.TextShadowColor};
    DrawCachedUIText(graphics, line1Params);

    Gdiplus::RectF line2(0, drawY - 200, w, h);
    DrawTextParams line2Params = {.text = i18n.AuthorInfo,
                                  .font = &font,
                                  .rect = &line2,
                                  .horizontalAlign = Gdiplus::StringAlignmentCenter,
                                  .alpha = alpha,
                                  .color = &colors.TextColor,
                                  .shadowColor = &colors.TextShadowColor};
    DrawCachedUIText(graphics, line2Params);

    Gdiplus::RectF line3(0, drawY - 100, w, h);
    DrawTextParams line3Params = {.text = i18n.DonateThank,
                                  .font = &font,
                                  .rect = &line3,
                                  .horizontalAlign = Gdiplus::StringAlignmentCenter,
                                  .alpha = alpha,
                                  .color = &colors.TextColor,
                                  .shadowColor = &colors.TextShadowColor};
    DrawCachedUIText(graphics, line3Params);
}
