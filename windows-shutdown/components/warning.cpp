#include "warning.h"

#include "consts/effects.h"
#include "consts/color-set.h"
#include "consts/font-style.h"
#include "i18n.h"
#include "ui.h"

void DrawWarning(Gdiplus::Graphics& graphics, BYTE alpha, int w, int h, std::wstring text) {
    if (alpha == 0) {
        return;
    }

    // static auto& warnings = AppState::GetInstance().config.warnings;
    // text = i18n.GetConfigWarningText(warnings)
    static auto& i18n = I18N::GetInstance();
    static auto& colors = ColorSet::GetInstance();
    static Gdiplus::FontFamily fontFamily(i18n.FontFamilyName.c_str());

    Gdiplus::Font warnFont(&fontFamily, INSTRUCTION_FONT_SIZE, Gdiplus::FontStyleBold);
    Gdiplus::RectF warnRect(CFG_WARNING_X, CFG_WARNING_Y, w, h);
    DrawTextParams warnParams = {.text = text,
                                 .font = &warnFont,
                                 .rect = &warnRect,
                                 .horizontalAlign = Gdiplus::StringAlignmentNear,
                                 .alpha = alpha,
                                 .color = &colors.TextWarnColor,
                                 //  .color = &colors.TextDangerColor,
                                 .shadowColor = &colors.TextShadowColor};
    DrawCachedUIText(graphics, warnParams);
}
