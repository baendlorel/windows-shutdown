#pragma once
// #define REALIFY(x) static_cast<Gdiplus::REAL>(x)
// #define INTIFY(x) static_cast<int>(x)

inline float REALIFY(int x) {
    return static_cast<Gdiplus::REAL>(x);
}

inline int INTIFY(float x) {
    return static_cast<int>(x);
}
