#include "utils.string.h"

static constexpr char ASCII_UPLOWER_DELTA = 32;

std::string trim(const std::string &s) {
    if (s.empty()) {
        return s;
    }

    auto start = s.begin();
    while (start != s.end() && std::isspace(static_cast<unsigned char>(*start))) {
        ++start;
    }
    auto end = s.end();
    do {
        --end;
    } while (end >= start && std::isspace(static_cast<unsigned char>(*end)));

    return (start <= end) ? std::string(start, end + 1) : "";
}

std::string to_uppercase(const std::string &str) {
    std::string result = str;

    for (char &ch : result) {
        if (ch >= 'a' && ch <= 'z') {
            ch -= ASCII_UPLOWER_DELTA;
        }
    }

    return result;
}

std::string to_lowercase(const std::string &str) {
    std::string result = str;

    for (char &ch : result) {
        if (ch >= 'A' && ch <= 'Z') {
            ch += ASCII_UPLOWER_DELTA;
        }
    }

    return result;
}
