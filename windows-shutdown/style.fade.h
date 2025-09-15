#pragma once

namespace fade {
enum class State : unsigned char { None, FadingIn, FadingOut };

// effects
inline constexpr unsigned char MAX_ALPHA = 255;

// 1/255
inline constexpr float ONE_TO_MAX_ALPHA = 1.0f / 255.0f;
inline constexpr int DURATION = 300;  // ms
// & It is no need to be 10, since 30 fps(33ms/frame) is enough
inline constexpr int FRAME_TIME = 30;
inline constexpr int TIMER_ID = 1001;

// Convert alpha (0..255) to opacity (0.0..1.0)
inline float get_opacity(const BYTE alpha) {
    return static_cast<float>(alpha) * ONE_TO_MAX_ALPHA;
}

}  // namespace fade
