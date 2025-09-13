import os
import re

# 控制字符和不可见字符的正则表达式（排除换行符和回车符）
control_char_regex = re.compile(r'[\x00-\x08\x0E-\x1F\x7F]')

def check_file_for_control_chars(file_path):
    """检查文件中的控制字符和不可见字符，忽略 LF 和 CRLF"""
    with open(file_path, 'r', encoding='utf-8', errors='ignore') as file:
        line_no = 1
        for line in file:
            # 去除 LF 和 CRLF 后，检查控制字符
            line_without_newlines = line.replace('\n', '').replace('\r', '')
            if control_char_regex.search(line_without_newlines):
                print(f"文件: {file_path}, 行号: {line_no} - 包含控制字符或不可见字符")
            line_no += 1

def traverse_directory(directory):
    """遍历目录，检查所有文本文件"""
    for root, dirs, files in os.walk(directory):
        for file in files:
            if file.endswith('.h') or file.endswith('.cpp'):  # 可以根据需要修改文件扩展名
                print(f"检查文件: {file}")
                file_path = os.path.join(root, file)
                check_file_for_control_chars(file_path)

if __name__ == "__main__":
    traverse_directory("windows-shutdown")

