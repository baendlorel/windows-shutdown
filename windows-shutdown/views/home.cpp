#include "home.h"
#include "components/instruction.h"
#include "consts/button-style.h"

#include "consts/effects.h"
#include "consts/color-set.h"
#include "app-state.h"
#include "i18n.h"
#include "ui.h"

void HomeView::DrawView(Gdiplus::Graphics& graphics, int w, int h) {
    BYTE alpha = appState.page.GetPageAlpha(Page::Home);

    // Draw image buttons (original logic)
    for (int i = 0; i < appState.buttons.size(); ++i) {
        auto& b = appState.buttons[i];
        int x = b.x - BUTTON_RADIUS;
        int y = b.y - BUTTON_RADIUS;

        auto imgAttr = ImageAttrWithAlpha(b.png, alpha);

        // where and what size to draw
        Gdiplus::Rect rect(x, y, BUTTON_DIAMETER, BUTTON_DIAMETER);

        // x, y, w, h cut from the source image
        // Since button images are 512x512, appState.buttons[i].png->GetWidth() is acutally 512
        graphics.DrawImage(b.png, rect, 0, 0, b.png->GetWidth(), b.png->GetHeight(),
                           Gdiplus::UnitPixel, imgAttr.get());

        // If hovered, overlay a semi-transparent white, but scaled by overall alpha
        if (i == appState.hoveredIndex) {
            Gdiplus::Color blended(ApplyAlpha(&colors.ButtonHighlightColor, alpha));
            Gdiplus::SolidBrush highlightBrush(blended);
            graphics.FillEllipse(&highlightBrush, x + BUTTON_SHADOW_WIDTH, y + BUTTON_SHADOW_WIDTH,
                                 BUTTON_DIAMETER - BUTTON_SHADOW_WIDTH * 2,
                                 BUTTON_DIAMETER - BUTTON_SHADOW_WIDTH * 2);
        }
    }

    // Draw exit instruction below buttons
    int instrY = (h / 2) + BUTTON_RADIUS + BUTTON_MARGIN_TOP + BUTTON_MARGIN_BOTTOM;
    Gdiplus::RectF instrRect(0, instrY, w, h);
    DrawInstruction(graphics, alpha, &instrRect, i18n.PressAnyKeyToExit);
}
