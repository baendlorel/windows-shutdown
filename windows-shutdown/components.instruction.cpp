#include "components.instruction.h"

#include "style.font.h"
#include "style.color.h"
#include "app.core.h"

#include "mini-ui.h"

// & Only draw instruction needs internal state check
void draw_instruction(Gdiplus::Graphics& graphics, const BYTE alpha, Gdiplus::RectF* rect,
                      const std::wstring& text) {
    if (alpha == 0) {
        return;
    }
    static bool showInstruction = app::config.instruction == cfg::Instruction::Show;

    static Gdiplus::FontFamily fontFamily(app::i18n.FontFamilyName.c_str());
    static Gdiplus::Font font(&fontFamily, font_style::INSTRUCTION_FONT_SIZE,
                              Gdiplus::FontStyleBold);

    if (!showInstruction) {
        return;
    }

    painter::draw_cached_text(graphics, {.text = text,
                                         .font = &font,
                                         .rect = rect,
                                         .horizontal_align = Gdiplus::StringAlignmentCenter,
                                         .alpha = alpha,
                                         .color = &color_set::TEXT_LIGHT,
                                         .shadow_color = &color_set::TEXT_SHADOW});
}
