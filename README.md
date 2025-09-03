# VS Code Project Configuration Guide

This project has been converted from a Visual Studio project to a VS Code project.

## Prerequisites

1. **Install VS Code**
2. **Install C/C++ Extension** (Microsoft C/C++)
3. **Install MSVC Compiler** (Visual Studio 2022 Community or Build Tools)
4. **Configure environment variables**

## Quick Start

### Environment Setup

The project is configured for the following paths:

- **Visual Studio 2022**: `D:\Program Files\Microsoft Visual Studio\2022\Community`
- **Windows Kits**: `D:\Windows Kits\10`
- **MSVC Version**: `14.44.35207`
- **Windows SDK Version**: `10.0.26100.0`

You have two options to set up the environment:

#### Option 1: Use the provided batch script

```batch
# Run the setup script in the project root
setup_env_and_vscode.bat
```

#### Option 2: Manual setup

```powershell
# Set environment variables in PowerShell
$env:INCLUDE="D:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\include;D:\Windows Kits\10\Include\10.0.26100.0\ucrt;D:\Windows Kits\10\Include\10.0.26100.0\shared;D:\Windows Kits\10\Include\10.0.26100.0\um;D:\Windows Kits\10\Include\10.0.26100.0\winrt"

$env:LIB="D:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\lib\x64;D:\Windows Kits\10\lib\10.0.26100.0\ucrt\x64;D:\Windows Kits\10\lib\10.0.26100.0\um\x64"

# Then start VS Code
code .
```

### Building the Project

1. **Use keyboard shortcut**: `Ctrl+Shift+P` -> `Tasks: Run Build Task` or `Ctrl+Shift+B`
2. **Select build task**:
   - `Full Build` - Recommended, includes resource file compilation
   - `C/C++: Build Debug` - Build C++ code only
   - `C/C++: Build Release` - Release version build

### Debugging the Project

1. **Set breakpoints**: Click on the line numbers in the editor
2. **Start debugging**: Press `F5` or `Ctrl+F5`
3. **Select debug configuration**:
   - `Debug windows-shutdown` - Automatically builds and debugs
   - `Debug windows-shutdown (No Build)` - Debug existing binary

## Project Structure

```
windows-shutdown/
├── .vscode/                    # VS Code configuration
│   ├── tasks.json             # Build task configuration
│   ├── launch.json            # Debug configuration
│   ├── c_cpp_properties.json  # C++ IntelliSense configuration
│   └── settings.json          # Workspace settings
├── windows-shutdown/          # Source code directory
│   ├── *.cpp                  # C++ source files
│   ├── *.h                    # Header files
│   ├── windows-shutdown.rc    # Resource file
│   └── assets/                # Asset files (icons, etc.)
├── x64/                       # Build output
│   ├── Debug/                 # Debug build output
│   └── Release/               # Release build output
├── setup_env_and_vscode.bat  # Environment setup script
└── README_VSCODE.md          # This guide
```

## Troubleshooting

### 1. Compiler Not Found

**Error**: `'cl.exe' is not recognized as an internal or external command`

**Solution**: Make sure VS Code is started with the proper environment variables set.

### 2. Resource Compilation Failed

**Error**: Resource file related errors

**Solution**:

1. Ensure `rc.exe` is in PATH
2. Check resource file paths
3. Run `Compile Resource Files` task separately

### 3. Library Linking Errors

**Error**: Cannot find certain `.lib` files

**Solution**: Ensure Windows SDK is properly installed and configured.

### 4. IntelliSense Not Working

**Solution**:

1. Check paths in `c_cpp_properties.json`
2. Restart VS Code
3. Run command `C/C++: Reset IntelliSense Database`

## Recommended Extensions

- **C/C++** (Microsoft) - Required
- **C/C++ Themes** - Syntax highlighting
- **Error Lens** - Enhanced error display
- **GitLens** - Git enhancements

## Manual Compilation

If VS Code tasks have issues, you can compile manually:

```cmd
# Set environment first (see setup_env_and_vscode.bat)

# Compile resource file
rc /fo x64\Debug\windows-shutdown.res windows-shutdown\windows-shutdown.rc

# Compile C++ code
cl /std:c++20 /EHsc /W3 /MD /D_WINDOWS /DUNICODE /D_UNICODE /Zi ^
   /Fe:x64\Debug\windows-shutdown.exe /Fo:x64\Debug\ /Fd:x64\Debug\ ^
   windows-shutdown\*.cpp ^
   /link x64\Debug\windows-shutdown.res user32.lib gdi32.lib gdiplus.lib ^
   kernel32.lib shell32.lib comctl32.lib advapi32.lib ole32.lib ^
   /SUBSYSTEM:WINDOWS /DEBUG
```

## Notes

- All comments in the code and configuration files use English
- The project uses C++20 standard
- Debug information is included for debugging support
- Resource files are automatically compiled as part of the build process
