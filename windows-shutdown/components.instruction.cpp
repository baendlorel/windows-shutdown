#include "components.instruction.h"

#include "style.font.h"
#include "style.color.h"
#include "config.h"
#include "i18n.h"
#include "mini-ui.h"

// & Only draw instruction needs internal state check
void DrawInstruction(Gdiplus::Graphics& graphics, BYTE alpha, Gdiplus::RectF* rect,
                     const std::wstring& text) {
    if (alpha == 0) {
        return;
    }

    static bool showInstruction = Config::GetInstance().instruction == Instruction::Show;
    if (!showInstruction) {
        return;
    }
    static auto& colors = ColorSet::GetInstance();
    static Gdiplus::FontFamily fontFamily(I18N::GetInstance().FontFamilyName.c_str());
    static Gdiplus::Font font(&fontFamily, INSTRUCTION_FONT_SIZE, Gdiplus::FontStyleBold);
    DrawTextParams instrParams = {.text = text,
                                  .font = &font,
                                  .rect = rect,
                                  .horizontalAlign = Gdiplus::StringAlignmentCenter,
                                  .alpha = alpha,
                                  .color = &colors.TextColor,
                                  .shadowColor = &colors.TextShadowColor};
    DrawCachedUIText(graphics, instrParams);
}
