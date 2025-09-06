#pragma once

enum class FadeState { None, FadingIn, FadingOut };

// effects
constexpr BYTE TARGET_ALPHA = 255;
constexpr int MAX_LOADSTRING = 100;   // ms
constexpr int FADEIN_DURATION = 200;  // ms
constexpr int FRAME_TIME = 20;  // & It is no need to be 10, since 30 fps(33ms/frame) is enough
constexpr int FADEIN_TIMER_ID = 1001;
constexpr int FADEOUT_TIMER_ID = 1002;
constexpr int COUNTDOWN_TIMER_ID = 1003;  // New timer for countdown
