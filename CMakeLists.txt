cmake_minimum_required(VERSION 3.0)
project(Tetris)

# Use GCC as the compiler (if preferred)
set(CMAKE_C_COMPILER gcc)

# Include Raylib headers and link libraries
include_directories(/opt/homebrew/Cellar/raylib/5.5/include)
link_directories(/opt/homebrew/Cellar/raylib/5.5/lib)

# Add your source file
add_executable(tetris TETRIS.c)

# Link Raylib
target_link_libraries(tetris raylib)
