import os

def to_utf8_bom(file_path):
    with open(file_path, 'rb') as f:
        content = f.read()
    # 检查是否已经是UTF-8 BOM
    if content.startswith(b'\xef\xbb\xbf'):
        return
    # 尝试用utf-8解码
    try:
        text = content.decode('utf-8')
    except UnicodeDecodeError:
        # 如果不是utf-8编码，按本地编码解码（如gbk），再转为utf-8
        try:
            text = content.decode('gbk')
        except Exception:
            print(f"跳过无法识别编码的文件: {file_path}")
            return
    # 重新写入为utf-8 BOM
    with open(file_path, 'wb') as f:
        f.write(b'\xef\xbb\xbf')
        f.write(text.encode('utf-8'))

def traverse_and_convert(directory):
    for root, dirs, files in os.walk(directory):
        for file in files:
            if file.endswith('.h') or file.endswith('.cpp'):
                file_path = os.path.join(root, file)
                print(f"转换: {file_path}")
                to_utf8_bom(file_path)

if __name__ == "__main__":
    traverse_and_convert("windows-shutdown")
