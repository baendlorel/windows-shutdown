#pragma once
#include <string>

// 2-step macro to expand macro value
#define WIDEN2(x) L##x
#define WIDEN(x)  WIDEN2(x)

std::string trim(const std::string& s);

std::string to_uppercase(const std::string& str);

std::string to_lowercase(const std::string& str);
