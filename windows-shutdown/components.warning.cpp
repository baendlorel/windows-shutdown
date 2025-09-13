#include "components.warning.h"

#include "style.fade.h"
#include "style.color.h"
#include "style.font.h"
#include "app.core.h"

#include "mini-ui.h"

void DrawWarning(Gdiplus::Graphics& graphics, BYTE alpha, Gdiplus::REAL w, Gdiplus::REAL h,
                 std::wstring text) {
    if (alpha == 0) {
        return;
    }

    static auto& i18n = App::GetInstance().i18n;
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
                                 .shadowColor = &colors.TextShadowColor};
    DrawCachedUIText(graphics, warnParams);
}
