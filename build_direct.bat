@echo off
REM Direct build script for Professional Text Editor without CMake
REM For use specifically with the Microsoft Developer Command Prompt

setlocal

echo Professional Text Editor Direct Build Script
echo ===========================================
echo.

REM Check if we're in a Visual Studio Developer Command Prompt
if "%VSCMD_ARG_TGT_ARCH%"=="" (
    echo This script must be run from a Visual Studio Developer Command Prompt.
    echo Please open a Developer Command Prompt and try again.
    exit /b 1
)

echo Building text editor directly with Microsoft Visual C++ Compiler...
echo.

REM Create build directory if it doesn't exist
if not exist build\direct mkdir build\direct

REM Set include directories
set INCLUDE_DIRS=/I"include"

REM Set compiler options
set COMPILE_OPTIONS=/nologo /W4 /WX- /sdl /GS /Gy /O2 /std:c11 /D "_CRT_SECURE_NO_WARNINGS"

REM List all source files
set SOURCE_FILES=src\main.c src\window.c src\control.c src\fileops.c

REM Compile
echo Compiling source files...
cl %COMPILE_OPTIONS% %INCLUDE_DIRS% %SOURCE_FILES% /Fe:"build\direct\editor.exe" /link user32.lib gdi32.lib comdlg32.lib kernel32.lib

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo Build failed. Please check the error messages above.
    exit /b 1
)

echo.
echo Build successful!
echo The executable is located at: build\direct\editor.exe
echo.
echo You can run the editor by typing: build\direct\editor.exe

exit /b 0
