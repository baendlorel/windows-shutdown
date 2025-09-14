#pragma once
#include <string>
#include <string_view>

// 2-step macro to expand macro value
#define WIDEN2(x) L##x
#define WIDEN(x)  WIDEN2(x)

std::string trim(const std::string& s);

std::string to_uppercase(const std::string& str);

std::string to_lowercase(const std::string& str);

// Helper to convert narrow string-like values to wide strings. Simple character-wise
// widening is sufficient because config keys/values are ASCII identifiers.
inline std::wstring widen(std::string_view s) {
    return std::wstring(s.begin(), s.end());
}

inline std::wstring widen(const std::string& s) {
    return std::wstring(s.begin(), s.end());
}
