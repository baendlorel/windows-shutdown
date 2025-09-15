#include "components.warning.h"

#include "consts.warning.h"
#include "style.color.h"
#include "style.font.h"
#include "app.core.h"

#include "mini-ui.h"

void draw_warning(Gdiplus::Graphics& graphics, const BYTE alpha, const Gdiplus::REAL w,
                  const Gdiplus::REAL h, const std::wstring& text) {
    if (alpha == 0) {
        return;
    }

    static Gdiplus::FontFamily fontFamily(app::i18n.FontFamilyName.c_str());

    Gdiplus::Font warnFont(&fontFamily, INSTRUCTION_FONT_SIZE, Gdiplus::FontStyleBold);
    Gdiplus::RectF warnRect(warning::DISPLAY_X, warning::DISPLAY_Y, w, h);
    painter::draw_cached_text(graphics, {.text = text,
                                         .font = &warnFont,
                                         .rect = &warnRect,
                                         .horizontal_align = Gdiplus::StringAlignmentNear,
                                         .alpha = alpha,
                                         .color = &color_set::TEXT_WARN,
                                         .shadow_color = &color_set::TEXT_SHADOW});
}
