#pragma once


enum class FadeState { None, FadingIn, FadingOut };

// effects
constexpr  unsigned char  MAX_ALPHA = 255;
constexpr int MAX_LOADSTRING = 100;  // ms
constexpr int FADE_DURATION = 240;   // ms
constexpr int FRAME_TIME = 30;       // & It is no need to be 10, since 30 fps(33ms/frame) is enough
constexpr int FADE_TIMER_ID = 1001;
constexpr int COUNTDOWN_TIMER_ID = 1002;  // New timer for countdown
#pragma once
