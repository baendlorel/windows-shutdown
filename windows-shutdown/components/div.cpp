// Base UI container implementation
#include "div.h"

bool Div::MouseHit(int mx, int my) const {
    return mx >= rect.X && mx <= rect.Y && my >= rect.GetTop() && my <= rect.GetBottom();
}

// bool Div::MouseHitCircle(int mx, int my) const {
//     float cx = rect.X + rect.Width / 2.0f;
//     float cy = rect.Y + rect.Height / 2.0f;
//     float radiusSquared = (rect.Width / 2.0f) * (rect.Width / 2.0f);
//     float dx = mx - cx;
//     float dy = my - cy;
//     return (dx * dx + dy * dy) <= radiusSquared;
// }
