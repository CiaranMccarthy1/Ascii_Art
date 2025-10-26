cmake_minimum_required(VERSION 3.10)
project(Ascii_ART LANGUAGES CXX)

# C++11 or higher
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add main executable
add_executable(ascii_art main.cpp)