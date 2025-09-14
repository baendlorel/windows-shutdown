#include "countdown.h"
#include "components.instruction.h"
#include "components.nixie-tube-clock.h"

#include "style.fade.h"
#include "style.color.h"
#include "style.font.h"

#include "mini-ui.h"
#include "dtm.h"

void CountdownView::DrawView(Gdiplus::Graphics& graphics, const DrawParams& params) {
    BYTE alpha = app.page.GetPageAlpha(Page::Countdown);
    static auto& countdownStyle = app.config.countdownStyle;

    if (!params.rect) {
        throw "rect为空";
    }

    int w = params.rect->Width;
    int h = params.rect->Height;

    // Check countdown style configuration
    if (countdownStyle == CFG::CountdownStyle::SteinsGate) {
        // Steins;Gate style with nixie tubes

        // First line: action description
        std::wstring firstLine = app.i18n.Wait(app.state.action, app.state.countdownSeconds);
        static Gdiplus::FontFamily fontFamily(app.i18n.FontFamilyName.c_str());
        Gdiplus::Font firstFont(&fontFamily, COUNT_DOWN_FONT_SIZE, Gdiplus::FontStyleBold);
        Gdiplus::REAL y = h * 0.32f;
        Gdiplus::RectF firstRect(0, y, w, h);
        DrawTextParams firstParams = {.text = firstLine,
                                      .font = &firstFont,
                                      .rect = &firstRect,
                                      .horizontalAlign = Gdiplus::StringAlignmentCenter,
                                      .alpha = alpha,
                                      .color = &colors_.TextColor,
                                      .shadowColor = &colors_.TextShadowColor};
        DrawCachedUIText(graphics, firstParams);

        // Nixie tube clock in the center
        Gdiplus::RectF clockRect(w * 0.5f, y + 125, 0, 0);
        DrawNixieTubeClock(graphics, alpha, clockRect, Gdiplus::PointF(0.5f, 0.0f),
                           app.state.countdownSeconds);

        // Draw cancel instruction below
        Gdiplus::RectF smallRect(0, y + 520, w, h);
        DrawInstruction(graphics, alpha, &smallRect, app.i18n.PressAnyKeyToCancel);
        return;
    }

    // * Normal style (original)
    static Gdiplus::FontFamily fontFamily(app.i18n.FontFamilyName.c_str());
    static auto* mono = Gdiplus::FontFamily::GenericMonospace();

    // First line: original style (left/right language preserved by i18n.Wait)
    std::wstring firstLine = app.i18n.Wait(app.state.action, app.state.countdownSeconds);

    // Fonts
    Gdiplus::Font firstFont(&fontFamily, COUNT_DOWN_FONT_SIZE, Gdiplus::FontStyleBold);
    // Position first line a bit above center
    Gdiplus::REAL y = h * 0.36f;
    Gdiplus::RectF firstRect(0, y, w, h);
    DrawTextParams firstParams = {.text = firstLine,
                                  .font = &firstFont,
                                  .rect = &firstRect,
                                  .horizontalAlign = Gdiplus::StringAlignmentCenter,
                                  .alpha = alpha,
                                  .color = &colors_.TextColor,
                                  .shadowColor = &colors_.TextShadowColor};
    DrawCachedUIText(graphics, firstParams);

    // Second line: large centered numeric seconds
    std::wstring secondLine = format_time(app.state.countdownSeconds) + app.i18n.Waiting[2];
    // Position second (number) centered below the first line
    Gdiplus::Font secondFont(mono, COUNT_DOWN_NUMBER_FONT_SIZE, Gdiplus::FontStyleBold);
    Gdiplus::RectF secondRect(0, y + 140, w, h);
    DrawTextParams secondParams = {.text = secondLine,
                                   .font = &secondFont,
                                   .rect = &secondRect,
                                   .manualAlign = false,
                                   .horizontalAlign = Gdiplus::StringAlignmentCenter,
                                   .alpha = alpha,
                                   .color = &colors_.TextWarnColor,
                                   .shadowColor = &colors_.TextShadowColor};
    // & seconds cannot use cache since it changes every second
    DrawUIText(graphics, secondParams);

    // Draw cancel instruction below the number
    Gdiplus::RectF smallRect(0, y + 300, w, h);
    DrawInstruction(graphics, alpha, &smallRect, app.i18n.PressAnyKeyToCancel);
}
