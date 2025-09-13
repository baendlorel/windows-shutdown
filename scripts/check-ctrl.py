import os
import re
import codecs

# 控制字符和不可见字符的正则表达式（排除换行符和回车符）
control_char_regex = re.compile(r'[\x00-\x08\x0E-\x1F\x7F]')

def detect_encoding(file_path):
    """简单检测文件编码（只区分 UTF-8 BOM/无 BOM/GBK/ANSI）"""
    with open(file_path, 'rb') as f:
        raw = f.read(4)
        if raw.startswith(codecs.BOM_UTF8):
            return "UTF-8 BOM"
        elif raw.startswith(codecs.BOM_UTF16_LE):
            return "UTF-16 LE BOM"
        elif raw.startswith(codecs.BOM_UTF16_BE):
            return "UTF-16 BE BOM"
        else:
            try:
                with open(file_path, 'r', encoding='utf-8') as testf:
                    testf.read()
                return "UTF-8"
            except UnicodeDecodeError:
                return "GBK/ANSI或其他"

def check_file_for_control_chars(file_path):
    """检查文件中的控制字符和不可见字符，忽略 LF 和 CRLF"""
    encoding = detect_encoding(file_path)
    print(f"文件: {file_path}, 编码: {encoding}")
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