# Minimum CMake version required to configure
cmake_minimum_required(VERSION 3.20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Define project name and enable c++
project(chess LANGUAGES CXX)

# Request cmake to generate build files with C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define executable target
add_executable(chess
    src/main.cpp

    src/ui/board_view.cpp
    src/ui/input_controller.cpp
    
    src/core/Position.cpp
    src/core/Attacks.cpp
    src/core/MoveGenerator.cpp

    src/core/magic/Magic.cpp

    src/config/DebugConfig.cpp
)

# Development tool for generating magic numbers
add_executable(magic_generator
    tools/MagicGenerator.cpp
)

# Allow #include "chess/..." from include/ directory
# From any src compiled into chess yoiu can inlude chess/... files
target_include_directories(chess PRIVATE include)
target_include_directories(magic_generator PRIVATE include)

# Enable compiler warnings
if (MSVC)
    # VSCODE comiler warnings
    target_compile_options(chess PRIVATE /W4 /permissive-)
    target_compile_options(magic_generator PRIVATE /W4 /permissive-)
else ()
    # GCC / CLANG
    target_compile_options(chess PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(magic_generator PRIVATE -Wall -Wextra -Wpedantic)
endif()


# --- SFML Integration (SFML 2.x)
if (APPLE)
    # Help CMake find Homebrew's keg-only sfml@2 package on macOS.
    list(APPEND CMAKE_PREFIX_PATH
        /opt/homebrew/opt/sfml@2
        /usr/local/opt/sfml@2
    )
endif()

find_package(SFML 2.5 REQUIRED COMPONENTS graphics window system)

target_link_libraries(chess PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)
