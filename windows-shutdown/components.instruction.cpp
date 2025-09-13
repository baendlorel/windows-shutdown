#include "components.instruction.h"

#include "style.font.h"
#include "style.color.h"
#include "app.core.h"

#include "mini-ui.h"

// & Only draw instruction needs internal state check
void DrawInstruction(Gdiplus::Graphics& graphics, BYTE alpha, Gdiplus::RectF* rect,
                     const std::wstring& text) {
    if (alpha == 0) {
        return;
    }
    static auto& app = App::GetInstance();
    static bool showInstruction = app.config.instruction == Instruction::Show;
    static auto& colors = ColorSet::GetInstance();
    static Gdiplus::FontFamily fontFamily(app.i18n.FontFamilyName.c_str());
    static Gdiplus::Font font(&fontFamily, INSTRUCTION_FONT_SIZE, Gdiplus::FontStyleBold);

    if (!showInstruction) {
        return;
    }

    DrawTextParams instrParams = {.text = text,
                                  .font = &font,
                                  .rect = rect,
                                  .horizontalAlign = Gdiplus::StringAlignmentCenter,
                                  .alpha = alpha,
                                  .color = &colors.TextColor,
                                  .shadowColor = &colors.TextShadowColor};
    DrawCachedUIText(graphics, instrParams);
}
