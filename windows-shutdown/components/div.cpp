// Base UI container implementation
#include "div.h"

bool Div::MouseHit(int mx, int my) const {
    return mx >= rect.X && mx <= rect.Y && my >= rect.GetTop() && my <= rect.GetBottom();
}
