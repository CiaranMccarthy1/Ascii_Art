# 🖼️ ASCII Art Generator (C++)

A lightweight command-line program that converts images into ASCII art using pixel brightness mapping.

Built in modern **C++11** and **CMake**, with image loading handled by the header-only [`stb_image`](https://github.com/nothings/stb) library.

---

## Features
- Converts any JPG/PNG/BMP image into ASCII characters
- Adjustable output width and height
- Cross-platform (Windows, Linux, macOS)
- Zero dependencies — just `stb_image.h` and a C++ compiler

---

## ⚙️ Build Instructions

### Clone and build with CMake
```bash
git clone https://github.com/<CiaranMccarthy1>/ASCII_ART.git
cd ASCII_ART
mkdir build && cd build
cmake ..
cmake --build .