#pragma once
#include <functional>
#include <string>

std::string read_file_content(const std::string& path);

bool save_to_file(const std::string& path, const std::string& content);

void for_each_line(const std::string& content, const std::function<void(std::string)>& callback);
