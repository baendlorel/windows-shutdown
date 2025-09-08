#include "countdown.h"
#include "components/instruction.h"

#include "consts/effects.h"
#include "consts/color-set.h"
#include "consts/font-style.h"
#include "app-state.h"
#include "i18n.h"
#include "ui.h"

void DrawDonate(Gdiplus::Graphics& graphics, BYTE alpha, int w, int h) {
    if (alpha == 0) {
        return;
    }
}
