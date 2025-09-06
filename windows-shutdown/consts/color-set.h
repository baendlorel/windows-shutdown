#pragma once
// order below must be preserved
#include <objidl.h>
#include <gdiplus.h>

// ui/colors
constexpr int DEFAULT_ALPHA = 52;

struct ColorSet {
    static ColorSet& GetInstance() {
        static ColorSet instance;
        return instance;
    }

    Gdiplus::Color BackgroundColor = Gdiplus::Color(DEFAULT_ALPHA, 0, 0, 0);
    Gdiplus::Color TextColor = Gdiplus::Color(255, 255, 255, 255);
    Gdiplus::Color TextWarnColor = Gdiplus::Color(255, 255, 193, 7);
    Gdiplus::Color TextDangerColor = Gdiplus::Color(255, 255, 151, 163);
    Gdiplus::Color TextShadowColor = Gdiplus::Color(128, 0, 0, 0);
    Gdiplus::Color ButtonHighlightColor = Gdiplus::Color(DEFAULT_ALPHA, 255, 255, 255);
};