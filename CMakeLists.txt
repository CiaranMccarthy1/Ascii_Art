cmake_minimum_required(VERSION 3.10)
project(Ascii_ART LANGUAGES CXX)

# C++17 or higher
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directory for headers (stb_image.h)
include_directories(lib)

# Add main executable from src directory
add_executable(ascii_art src/main.cpp)