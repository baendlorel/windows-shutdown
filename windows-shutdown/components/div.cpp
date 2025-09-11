// Base UI container implementation
#include "div.h"

bool Div::MouseHitRect(int mx, int my) const {
    if (!visible) {
        return false;
    }

    return mx >= rect.GetLeft() && mx <= rect.GetRight() && my >= rect.GetTop() &&
           my <= rect.GetBottom();
}

bool Div::MouseHitCircle(int mx, int my) const {
    if (!visible) {
        return false;
    }
    static float cx = rect.GetLeft() + rect.Width / 2.0f;
    static float cy = rect.GetTop() + rect.Height / 2.0f;
    static float radiusSquared = (rect.Width / 2.0f) * (rect.Width / 2.0f);
    float dx = mx - cx;
    float dy = my - cy;
    return (dx * dx + dy * dy) <= radiusSquared;
}
