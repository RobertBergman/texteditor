cmake_minimum_required(VERSION 3.20)
project(ProfessionalTextEditor C)

# Set C standard to C11
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add compiler flags
if(MSVC)
    # MSVC compiler flags
    add_compile_options(/W4 /sdl /O2 /GS /DYNAMICBASE)
else()
    # GCC/Clang compiler flags
    add_compile_options(-Wall -Wextra -O2)
endif()

# Include directories
include_directories(include)

# Add source files
file(GLOB_RECURSE SOURCES "src/*.c")

# Define the executable
add_executable(editor ${SOURCES})

# Link libraries
target_link_libraries(editor PRIVATE
    user32
    gdi32
    comdlg32
    kernel32
    Comctl32 # Added for common controls (status bar)
    Shlwapi  # Added for PathFindFileName
)

# Set output directories
set_target_properties(editor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Install targets
install(TARGETS editor
    RUNTIME DESTINATION bin
)
