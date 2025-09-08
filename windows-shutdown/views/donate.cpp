#include "donate.h"

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

    // localized polite & cute thank-you line from i18n
    std::wstring thankText = i18n.DonateThank;

    Gdiplus::Bitmap* img = LoadDonateBitmap();

    // Draw image centered
    int imgW = img->GetWidth();
    int imgH = img->GetHeight();
    int drawX = (w - imgW) / 2;
    int drawY = (h - imgH) / 2;

    auto imgAttr = ImageAttrWithAlpha(img, alpha);
    Gdiplus::RectF imgRect(static_cast<Gdiplus::REAL>(drawX), static_cast<Gdiplus::REAL>(drawY),
                           static_cast<Gdiplus::REAL>(imgW), static_cast<Gdiplus::REAL>(imgH));
    graphics.DrawImage(img, imgRect, 0, 0, imgW, imgH, Gdiplus::UnitPixel, imgAttr.get());

    // Draw thank-you text above image
    Gdiplus::Font thankFont(&fontFamily, INSTRUCTION_FONT_SIZE + 2, Gdiplus::FontStyleBold);
    Gdiplus::RectF thankRect(0, static_cast<Gdiplus::REAL>(drawY) - 60.0f,
                             static_cast<Gdiplus::REAL>(w), static_cast<Gdiplus::REAL>(h));
    DrawTextParams thankParams = {.text = thankText,
                                  .font = &thankFont,
                                  .rect = &thankRect,
                                  .horizontalAlign = Gdiplus::StringAlignmentCenter,
                                  .alpha = alpha,
                                  .color = &colors.TextColor,
                                  .shadowColor = &colors.TextShadowColor};
    DrawCachedUIText(graphics, thankParams);
}
