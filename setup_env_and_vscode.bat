@echo off
REM Set up Visual Studio 2022 environment for command line compilation
REM This script sets up the necessary environment variables for VS Code

set "VSPATH=D:\Program Files\Microsoft Visual Studio\2022\Community"
set "WKITSPATH=D:\Windows Kits\10"
set "MSVCVER=14.44.35207"
set "WKITSVER=10.0.26100.0"

set "PATH=%VSPATH%\VC\Tools\MSVC\%MSVCVER%\bin\Hostx64\x64;%WKITSPATH%\bin\%WKITSVER%\x64;%PATH%"
set "INCLUDE=%VSPATH%\VC\Tools\MSVC\%MSVCVER%\include;%WKITSPATH%\Include\%WKITSVER%\ucrt;%WKITSPATH%\Include\%WKITSVER%\shared;%WKITSPATH%\Include\%WKITSVER%\um;%WKITSPATH%\Include\%WKITSVER%\winrt"
set "LIB=%VSPATH%\VC\Tools\MSVC\%MSVCVER%\lib\x64;%WKITSPATH%\lib\%WKITSVER%\ucrt\x64;%WKITSPATH%\lib\%WKITSVER%\um\x64"

echo Environment set up for Visual Studio 2022 compilation
echo Starting VS Code...
code .
