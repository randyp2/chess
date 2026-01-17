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
    src/core/Position.cpp
    src/ui/input_controller.cpp
    src/config/DebugConfig.cpp
)

# TEMP: force SFML 2 include path (to avoid SFML 3 headers)
target_include_directories(chess PRIVATE
    /opt/homebrew/opt/sfml@2/include
)

# Allow #include "chess/..." from include/ directory
# From any src compiled into chess yoiu can inlude chess/... files
target_include_directories(chess PRIVATE include)

# Enable compiler warnings
if (MSVC)
    # VSCODE comiler warnings
    target_compile_options(chess PRIVATE /W4 /permissive-)
else ()
    # GCC / CLANG
    target_compile_options(chess PRIVATE -Wall -Wextra -Wpedantic)
endif()


# --- SFML Integration (SFML 2.x)
find_package(SFML 2.5 REQUIRED COMPONENTS graphics window system)

target_include_directories(chess PRIVATE
    /opt/homebrew/opt/sfml@2/include
)

target_link_libraries(chess PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)

