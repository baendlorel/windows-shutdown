#pragma once
#include <functional>
#include <string>

namespace fs {

std::string read_content(const std::string& path);

bool save_content(const std::string& path, const std::string& content);

void for_each_line(const std::string& content, const std::function<void(std::string)>& callback);

}  // namespace fs
