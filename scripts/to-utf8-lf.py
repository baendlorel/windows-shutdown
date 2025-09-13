import os

# 指定要处理的目录名称
TARGET_FOLDER_NAME = "windows-shutdown"

# 获取当前文件所在目录的路径
CURRENT_DIR = os.path.dirname(os.path.abspath(__file__))
# 构建目标目录的完整路径
TARGET_DIR = os.path.join(CURRENT_DIR, TARGET_FOLDER_NAME)

# 支持的文件扩展名，None 表示所有文件
SUPPORTED_EXTS = None

BOM = b'\xef\xbb\xbf'

def convert_file(filepath):
    """转换单个文件：去除BOM，统一换行符为LF，保存为UTF-8无BOM"""
    try:
        with open(filepath, 'rb') as f:
            content = f.read()
        
        # 去除 BOM
        if content.startswith(BOM):
            content = content[len(BOM):]
        
        try:
            text = content.decode('utf-8')
        except UnicodeDecodeError:
            # 跳过无法用UTF-8解码的文件（可能是二进制文件）
            print(f"跳过二进制文件: {filepath}")
            return
        
        # 替换所有行尾为 LF
        text = text.replace('\r\n', '\n').replace('\r', '\n')
        
        # 写回 UTF-8 无 BOM
        with open(filepath, 'w', encoding='utf-8', newline='\n') as f:
            f.write(text)
        
        print(f"已处理: {filepath}")
            
    except Exception as e:
        print(f"处理文件时出错 {filepath}: {e}")

def process_dir(root_dir):
    """处理目录中的所有文件"""
    if not os.path.exists(root_dir):
        print(f"错误: 目录不存在: {root_dir}")
        return
    
    if not os.path.isdir(root_dir):
        print(f"错误: 不是目录: {root_dir}")
        return
    
    file_count = 0
    for root, dirs, files in os.walk(root_dir):
        for name in files:
            if SUPPORTED_EXTS:
                if not any(name.lower().endswith(ext) for ext in SUPPORTED_EXTS):
                    continue
            
            filepath = os.path.join(root, name)
            convert_file(filepath)
            file_count += 1
    
    print(f"\n处理完成！共处理了 {file_count} 个文件")

if __name__ == '__main__':
    print(f"开始处理目录: {TARGET_DIR}")
    process_dir(TARGET_DIR)