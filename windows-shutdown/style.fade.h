#pragma once

namespace FADE {
enum class State { None, FadingIn, FadingOut };

// effects
inline constexpr unsigned char MAX_ALPHA = 255;
inline constexpr int DURATION = 240;  // ms
// & It is no need to be 10, since 30 fps(33ms/frame) is enough
inline constexpr int FRAME_TIME = 30;
inline constexpr int TIMER_ID = 1001;

}  // namespace FADE
