#include "utils.fs.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

std::string read_file_content(const std::string& path) {
    const std::ifstream file(path);

    if (!file.is_open()) {
        return {};
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

bool save_to_file(const std::string& path, const std::string& content) {
    std::ofstream file(path, std::ios::out | std::ios::trunc);

    if (!file.is_open()) {
        return false;
    }

    file << content;

    if (file.fail()) {
        return false;
    }

    file.close();
    return true;
}

void for_each_line(const std::string& content, const std::function<void(std::string)>& callback) {
    std::string line;

    for (const char ch : content) {
        if (ch == '\n') {
            callback(line);
            line.clear();
        } else {
            line += ch;
        }
    }

    if (!line.empty()) {
        callback(line);
    }
}