# Professional Text Editor

A professional text editor application built for Windows using the Win32 API, implementing C best practices with modern standards (C11) and a modular design.

## Features

* Clean, modular codebase with proper separation of concerns
* Proper memory management and error handling
* Complete menu with fully functional options:
  * **File**: New, Open, Save, Exit
  * **Edit**: Cut, Copy, Paste
  * **Help**: About
* Dynamically resizable text area that adjusts to window size
* Multi-line text editing with automatic scrolling
* Standard file open/save dialogs

## Project Structure

The project follows modern C development best practices with a clear directory structure:

```
text-editor/
├── include/           # Header files (.h)
│   ├── editor.h       # Common includes, constants, and declarations
│   ├── window.h       # Window management functionality
│   ├── control.h      # Edit control functionality
│   └── fileops.h      # File operations
├── src/               # Source files (.c)
│   ├── main.c         # Application entry point
│   ├── window.c       # Window implementation
│   ├── control.c      # Edit control implementation
│   └── fileops.c      # File operations implementation
├── build/             # Build output (generated)
├── docs/              # Documentation
└── CMakeLists.txt     # CMake build script
```

## Building the Project

This project provides multiple build options to accommodate different development environments.

### Option 1: Using the Build Scripts (Recommended)

The project includes two build scripts for Windows:

#### Using build.bat (CMake-based build)

This script automatically detects available compilers and builds the project with CMake:

1. Open a command prompt or PowerShell
2. Navigate to the project directory
3. Run:
   ```
   build.bat
   ```

The script will:
- Detect available compilers (Visual Studio or MinGW)
- Let you choose if multiple options are available
- Configure and build the project
- Report the location of the output executable

#### Using build_direct.bat (Direct MSVC build)

This script builds the project directly with the Visual C++ compiler:

1. Open a **Visual Studio Developer Command Prompt**
2. Navigate to the project directory
3. Run:
   ```
   build_direct.bat
   ```

### Option 2: Manual Build with CMake

For more control over the build process, you can use CMake directly.

#### Prerequisites

* CMake 3.20 or higher
* Visual Studio 2022 with C/C++ development tools, or MinGW-w64

#### Build Steps with Visual Studio

1. Create and navigate to the build directory:
   ```bash
   mkdir -p build
   cd build
   ```

2. Generate Visual Studio solution:
   ```bash
   cmake -G "Visual Studio 17 2022" -A Win32 ..
   ```

3. Build the project:
   ```bash
   cmake --build . --config Release
   ```

4. The executable will be in the `bin/Release` directory.

#### Build Steps with MinGW

1. Create and navigate to the build directory:
   ```bash
   mkdir -p build
   cd build
   ```

2. Generate Makefiles:
   ```bash
   cmake -G "MinGW Makefiles" ..
   ```

3. Build the project:
   ```bash
   cmake --build .
   ```

4. The executable will be in the `bin` directory.

### Option 3: Manual Build with Visual C++ Compiler

1. Open a Visual Studio Developer Command Prompt
2. Navigate to the project directory
3. Run:
   ```
   cl /std:c11 /W4 /sdl /GS /O2 /Iinclude src\main.c src\window.c src\control.c src\fileops.c /Fe:"editor.exe" /link user32.lib gdi32.lib comdlg32.lib kernel32.lib
   ```

## Code Quality

The code adheres to modern C best practices:

* Uses C11 standard
* Comprehensive error handling
* Memory safety with proper allocation/deallocation
* Clear function documentation with Doxygen-style comments
* Modular design with single-responsibility components
* Consistent naming and style conventions
* Security measures (bounds checking, input validation)

## Future Improvements

* Add syntax highlighting for programming languages
* Implement undo/redo functionality
* Add line numbering
* Support for different character encodings
* Find and replace functionality
* Add support for themes and customization
* Implement tabbed interface for multiple documents
