#include "countdown.h"
#include "components/instruction.h"
#include "components/nixie-tube-clock.h"

#include "consts/effects.h"
#include "consts/color-set.h"
#include "consts/font-style.h"
#include "app-state.h"
#include "i18n.h"
#include "ui.h"

void DrawCountdown(Gdiplus::Graphics& graphics, int w, int h) {
    static auto& appState = AppState::GetInstance();
    BYTE alpha = appState.page.GetPageAlpha(Page::Countdown);
    if (alpha == 0) {
        return;
    }

    static auto& i18n = I18N::GetInstance();
    static auto& colors = ColorSet::GetInstance();
    static auto& config = appState.config;

    // Check countdown style configuration
    if (config.countdownStyle == CountdownStyle::SteinsGate) {
        // Steins;Gate style with nixie tubes

        // First line: action description
        std::wstring firstLine = i18n.Wait(appState.action, appState.countdownSeconds);
        static Gdiplus::FontFamily fontFamily(i18n.FontFamilyName.c_str());
        Gdiplus::Font firstFont(&fontFamily, COUNT_DOWN_FONT_SIZE, Gdiplus::FontStyleBold);
        Gdiplus::REAL y = h * 0.32f;
        Gdiplus::RectF firstRect(0, y, w, h);
        DrawTextParams firstParams = {.text = firstLine,
                                      .font = &firstFont,
                                      .rect = &firstRect,
                                      .horizontalAlign = Gdiplus::StringAlignmentCenter,
                                      .alpha = alpha,
                                      .color = &colors.TextColor,
                                      .shadowColor = &colors.TextShadowColor};
        DrawCachedUIText(graphics, firstParams);

        // Nixie tube clock in the center
        Gdiplus::RectF clockRect(w * 0.2f, y + 120, w * 0.6f, 120);
        DrawNixieTubeClock(graphics, alpha, clockRect, Gdiplus::PointF(0.5f, 0.5f),
                           appState.countdownSeconds);

        // Draw cancel instruction below
        Gdiplus::RectF smallRect(0, y + 280, w, h);
        DrawInstruction(graphics, alpha, &smallRect, i18n.PressAnyKeyToCancel);
    } else {
        // Normal style (original)
        static Gdiplus::FontFamily fontFamily(i18n.FontFamilyName.c_str());
        static auto* mono = Gdiplus::FontFamily::GenericMonospace();

        // First line: original style (left/right language preserved by i18n.Wait)
        std::wstring firstLine = i18n.Wait(appState.action, appState.countdownSeconds);

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
                                      .color = &colors.TextColor,
                                      .shadowColor = &colors.TextShadowColor};
        DrawCachedUIText(graphics, firstParams);

        // Second line: large centered numeric seconds
        std::wstring secondLine = FormatTime(appState.countdownSeconds) + i18n.Waiting[2];
        // Position second (number) centered below the first line
        Gdiplus::Font secondFont(mono, COUNT_DOWN_NUMBER_FONT_SIZE, Gdiplus::FontStyleBold);
        Gdiplus::RectF secondRect(0, y + 140, w, h);
        DrawTextParams secondParams = {.text = secondLine,
                                       .font = &secondFont,
                                       .rect = &secondRect,
                                       .manualAlign = false,
                                       .horizontalAlign = Gdiplus::StringAlignmentCenter,
                                       .alpha = alpha,
                                       .color = &colors.TextWarnColor,
                                       .shadowColor = &colors.TextShadowColor};
        // & seconds cannot use cache since it changes every second
        DrawUIText(graphics, secondParams);

        // Draw cancel instruction below the number
        Gdiplus::RectF smallRect(0, y + 300, w, h);
        DrawInstruction(graphics, alpha, &smallRect, i18n.PressAnyKeyToCancel);
    }
}
