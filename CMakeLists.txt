cmake_minimum_required(VERSION 3.10)

# Project name
project(CHIP8_Emulator)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find SDL2 library
find_package(SDL2 REQUIRED)

# Add the executable
add_executable(${PROJECT_NAME} main.cpp chip8.cpp)

# Include SDL2 headers
include_directories(${SDL2_INCLUDE_DIRS})

# Link SDL2 library
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
