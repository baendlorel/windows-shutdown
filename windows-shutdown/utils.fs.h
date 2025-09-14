#pragma once
#include <functional>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

std::string read_file_content(const std::string& path);

bool save_to_file(const std::string& path, const std::string& content);

void for_each_line(const std::string& content, std::function<void(std::string)> callback);
