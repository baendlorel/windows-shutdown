#pragma once
inline float to_real(const int x) {
    return static_cast<float>(x);
}

inline float to_real(const unsigned int x) {
    return static_cast<float>(x);
}

inline float to_real(const unsigned char x) {
    return static_cast<float>(x);
}

inline int to_int(const float x) {
    return static_cast<int>(x);
}

inline unsigned char to_byte(const int x) {
    return static_cast<unsigned char>(x);
}
