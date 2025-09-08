#include "countdown.h"

#include "consts/effects.h"
#include "consts/color-set.h"
#include "consts/font-style.h"
#include "app-state.h"
#include "i18n.h"
#include "ui.h"

// (original empty stub removed)

// Load and cache donate bitmap from assets folder using I18N::DonateQrFileName
static Gdiplus::Bitmap* LoadDonateBitmap() {
    static Gdiplus::Bitmap* cached = nullptr;
    if (cached) {
        return cached;
    }

    auto& i18n = I18N::GetInstance();
    // assets path relative to executable / project. Assume assets are in "assets/" next to binary.
    std::wstring filePath = L"assets/" + i18n.DonateQrFileName;

    Gdiplus::Bitmap* img = Gdiplus::Bitmap::FromFile(filePath.c_str());
    if (!img || img->GetLastStatus() != Gdiplus::Ok) {
        // fail quietly and return nullptr
        delete img;
        cached = nullptr;
        return nullptr;
    }
    cached = img;
    return cached;
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
    if (!img) {
        // still draw the text centered if image not available
        Gdiplus::Font font(&fontFamily, INSTRUCTION_FONT_SIZE, Gdiplus::FontStyleBold);
        Gdiplus::RectF textRect(0, static_cast<Gdiplus::REAL>(h) / 2 - 50.0f,
                                static_cast<Gdiplus::REAL>(w), static_cast<Gdiplus::REAL>(h));
        DrawTextParams params = {.text = thankText,
                                 .font = &font,
                                 .rect = &textRect,
                                 .horizontalAlign = Gdiplus::StringAlignmentCenter,
                                 .alpha = alpha,
                                 .color = &colors.TextColor,
                                 .shadowColor = &colors.TextShadowColor};
        DrawCachedUIText(graphics, params);
        return;
    }

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
