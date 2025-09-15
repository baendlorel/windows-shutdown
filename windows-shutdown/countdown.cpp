#include "countdown.h"

#include <stdexcept>

#include "components.instruction.h"
#include "components.nixie-tube-clock.h"

#include "style.color.h"
#include "style.font.h"

#include "mini-ui.h"
#include "dtm.h"

void CountdownView::draw_view(Gdiplus::Graphics& graphics, const DrawParams& params) {
    BYTE alpha = app::page.get_page_alpha(app::Page::Countdown);
    static auto& countdownStyle = app::config.countdown_style;

    if (!params.rect) {
        throw std::runtime_error("rect为空");
    }

    const Gdiplus::REAL w = params.rect->Width;
    const Gdiplus::REAL h = params.rect->Height;

    // Check countdown style configuration
    if (countdownStyle == cfg::CountdownStyle::SteinsGate) {
        const Gdiplus::REAL y = h * 0.32f;

        // Steins;Gate style with nixie tubes
        static Gdiplus::FontFamily fontFamily(app::i18n.FontFamilyName.c_str());

        // First line: action description
        const std::wstring firstLine =
            app::i18n.wait(app::state.action, app::state.countdown_seconds);
        Gdiplus::Font firstFont(&fontFamily, font_style::COUNT_DOWN_FONT_SIZE,
                                Gdiplus::FontStyleBold);
        Gdiplus::RectF firstRect(0, y, w, h);

        painter::draw_cached_text(graphics, {.text = firstLine,
                                             .font = &firstFont,
                                             .rect = &firstRect,
                                             .horizontal_align = Gdiplus::StringAlignmentCenter,
                                             .alpha = alpha,
                                             .color = &color_set::TEXT_LIGHT,
                                             .shadow_color = &color_set::TEXT_SHADOW});

        // Nixie tube clock in the center
        const Gdiplus::RectF clockRect(w * 0.5f, y + 125, 0, 0);
        draw_nixie_tube_clock(graphics, alpha, clockRect, Gdiplus::PointF(0.5f, 0.0f),
                              app::state.countdown_seconds);

        // Draw cancel instruction below
        Gdiplus::RectF smallRect(0, y + 520, w, h);
        draw_instruction(graphics, alpha, &smallRect, app::i18n.PressAnyKeyToCancel);
        return;
    }

    // * Normal style (original)
    static Gdiplus::FontFamily fontFamily(app::i18n.FontFamilyName.c_str());
    static auto* mono = Gdiplus::FontFamily::GenericMonospace();

    // First line: original style (left/right language preserved by i18n.Wait)
    const std::wstring firstLine = app::i18n.wait(app::state.action, app::state.countdown_seconds);

    // Fonts
    Gdiplus::Font firstFont(&fontFamily, font_style::COUNT_DOWN_FONT_SIZE, Gdiplus::FontStyleBold);

    // Position first line a bit above center
    const Gdiplus::REAL y = h * 0.36f;
    Gdiplus::RectF firstRect(0, y, w, h);

    painter::draw_cached_text(graphics, {.text = firstLine,
                                         .font = &firstFont,
                                         .rect = &firstRect,
                                         .horizontal_align = Gdiplus::StringAlignmentCenter,
                                         .alpha = alpha,
                                         .color = &color_set::TEXT_LIGHT,
                                         .shadow_color = &color_set::TEXT_SHADOW});

    // Second line: large centered numeric seconds
    const std::wstring secondLine =
        format_time(app::state.countdown_seconds) + app::i18n.Waiting[2];

    // Position second (number) centered below the first line
    Gdiplus::Font secondFont(mono, font_style::COUNT_DOWN_NUMBER_FONT_SIZE, Gdiplus::FontStyleBold);
    Gdiplus::RectF secondRect(0, y + 140, w, h);

    // & seconds cannot use cache since it changes every second
    painter::draw_text(graphics, {.text = secondLine,
                                  .font = &secondFont,
                                  .rect = &secondRect,
                                  .manual_align = false,
                                  .horizontal_align = Gdiplus::StringAlignmentCenter,
                                  .alpha = alpha,
                                  .color = &color_set::TEXT_WARN,
                                  .shadow_color = &color_set::TEXT_SHADOW});

    // Draw cancel instruction below the number
    Gdiplus::RectF smallRect(0, y + 300, w, h);
    draw_instruction(graphics, alpha, &smallRect, app::i18n.PressAnyKeyToCancel);
}
