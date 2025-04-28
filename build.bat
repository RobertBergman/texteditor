@echo off
REM Build script for Professional Text Editor
REM Supports both Visual Studio and MinGW builds

setlocal EnableDelayedExpansion

echo Professional Text Editor Build Script
echo ====================================
echo.

REM Check if CMake is installed
where cmake >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo Error: CMake is not installed or not in PATH.
    echo Please install CMake 3.20 or higher from https://cmake.org/download/
    exit /b 1
)

REM Detect available compilers
set HAVE_VS=0
set HAVE_MINGW=0

REM Check for Visual Studio
cl /? >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    set HAVE_VS=1
    echo Visual Studio detected.
) else (
    REM Alternative detection method
    if exist "%VCToolsInstallDir%\bin\Hostx64\x86\cl.exe" (
        set HAVE_VS=1
        echo Visual Studio detected (through VCToolsInstallDir).
    ) else if exist "%VS2022INSTALLDIR%\VC\Auxiliary\Build\vcvars32.bat" (
        set HAVE_VS=1
        echo Visual Studio detected (VS2022 installation).
    ) else if exist "%VS2019INSTALLDIR%\VC\Auxiliary\Build\vcvars32.bat" (
        set HAVE_VS=1
        echo Visual Studio detected (VS2019 installation).
    )
)

REM Check for MinGW
where gcc >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    set HAVE_MINGW=1
    echo MinGW detected.
)

REM If running from Developer Command Prompt, assume Visual Studio is available
if "%VSCMD_ARG_TGT_ARCH%" NEQ "" (
    set HAVE_VS=1
    echo Visual Studio Developer Command Prompt detected.
)

REM If no compilers found, exit
if %HAVE_VS% EQU 0 if %HAVE_MINGW% EQU 0 (
    echo Error: No supported compilers found.
    echo Please install Visual Studio 2022 with C/C++ tools or MinGW-w64,
    echo or run this script from a Visual Studio Developer Command Prompt.
    exit /b 1
)

REM Ask user which compiler to use if both are available
set COMPILER=
if %HAVE_VS% EQU 1 if %HAVE_MINGW% EQU 1 (
    echo Multiple compilers detected. Please choose:
    echo 1. Visual Studio
    echo 2. MinGW
    set /p CHOICE="Enter choice (1 or 2): "
    
    if "%CHOICE%"=="1" (
        set COMPILER=vs
    ) else if "%CHOICE%"=="2" (
        set COMPILER=mingw
    ) else (
        echo Invalid choice. Exiting.
        exit /b 1
    )
) else if %HAVE_VS% EQU 1 (
    set COMPILER=vs
) else if %HAVE_MINGW% EQU 1 (
    set COMPILER=mingw
)

REM Create build directory if it doesn't exist
if not exist build mkdir build
cd build

REM Configure and build based on selected compiler
if "%COMPILER%"=="vs" (
    echo.
    echo Configuring with Visual Studio...
    
    REM Try to determine the Visual Studio version
    set VS_GENERATOR="Visual Studio 17 2022"
    
    REM If running from Developer Command Prompt, try to detect version
    if "%VisualStudioVersion%"=="17.0" (
        set VS_GENERATOR="Visual Studio 17 2022"
    ) else if "%VisualStudioVersion%"=="16.0" (
        set VS_GENERATOR="Visual Studio 16 2019"
    ) else if "%VisualStudioVersion%"=="15.0" (
        set VS_GENERATOR="Visual Studio 15 2017"
    )
    
    echo Using generator: %VS_GENERATOR%
    
    cmake -G %VS_GENERATOR% -A Win32 ..
    
    if %ERRORLEVEL% NEQ 0 (
        echo.
        echo CMake configuration failed. Trying fallback approach...
        cmake -G "Visual Studio 17 2022" -A Win32 ..
        
        if %ERRORLEVEL% NEQ 0 (
            echo.
            echo CMake configuration failed with fallback generator too.
            echo Please check your Visual Studio installation.
            exit /b 1
        )
    )
    
    echo.
    echo Building with Visual Studio...
    cmake --build . --config Release
    
    if %ERRORLEVEL% EQU 0 (
        echo.
        echo Build successful!
        echo The executable is located at: bin\Release\editor.exe
    ) else (
        echo.
        echo Build failed. Please check the error messages above.
        exit /b 1
    )
) else if "%COMPILER%"=="mingw" (
    echo.
    echo Configuring with MinGW...
    cmake -G "MinGW Makefiles" ..
    
    if %ERRORLEVEL% NEQ 0 (
        echo.
        echo CMake configuration failed.
        echo Please check your MinGW installation.
        exit /b 1
    )
    
    echo.
    echo Building with MinGW...
    cmake --build .
    
    if %ERRORLEVEL% EQU 0 (
        echo.
        echo Build successful!
        echo The executable is located at: bin\editor.exe
    ) else (
        echo.
        echo Build failed. Please check the error messages above.
        exit /b 1
    )
)

cd ..

echo.
echo You can now run the text editor from the build directory.

exit /b 0
