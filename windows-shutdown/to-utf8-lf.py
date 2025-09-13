import os

# 需要处理的目录
TARGET_DIR = os.path.dirname(os.path.abspath(__file__))

SUPPORTED_EXTS = None  # None 表示所有文件

BOM = b'\xef\xbb\xbf'

def convert_file(filepath):
    with open(filepath, 'rb') as f:
        content = f.read()
    # 去除 BOM
    if content.startswith(BOM):
        content = content[len(BOM):]
    try:
        text = content.decode('utf-8')
    except UnicodeDecodeError:
        # 跳过无法解码的文件
        return
    # 替换所有行尾为 LF
    text = text.replace('\r\n', '\n').replace('\r', '\n')
    # 写回 UTF-8 无 BOM
    with open(filepath, 'w', encoding='utf-8', newline='\n') as f:
        f.write(text)

def process_dir(root_dir):
    for root, dirs, files in os.walk(root_dir):
        for name in files:
            if SUPPORTED_EXTS:
                if not any(name.lower().endswith(ext) for ext in SUPPORTED_EXTS):
                    continue
            filepath = os.path.join(root, name)
            convert_file(filepath)

if __name__ == '__main__':
    process_dir(TARGET_DIR)
