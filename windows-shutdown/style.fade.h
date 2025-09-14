#pragma once

namespace FADE {
enum class State { None, FadingIn, FadingOut };

// effects
constexpr unsigned char MAX_ALPHA = 255;
constexpr int DURATION = 240;   // ms
constexpr int FRAME_TIME = 30;  // & It is no need to be 10, since 30 fps(33ms/frame) is enough
constexpr int TIMER_ID = 1001;

}  // namespace FADE
