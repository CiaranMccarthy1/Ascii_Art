# 🖼️ ASCII Art Generator (C++)

A lightweight command-line program that converts images into ASCII art using pixel brightness mapping.
Built in modern **C++17** and **CMake**, with image loading handled by the header-only [`stb_image`](https://github.com/nothings/stb) library.

---

## ✨ Features
- **High Quality**: Uses Box Sampling (pixel averaging) for smooth, accurate downscaling.
- **Color Support**: Option to output full-color ANSI ASCII art.
- **File Output**: Save your art directly to a text file.
- **Performance**: Optimized buffer handling for fast rendering.
- **Cross-platform**: Works on Windows, Linux, and macOS.
- **Zero dependencies**: Just `stb_image.h` and a C++ compiler.

---

## Usage

Run the program from the command line:

```bash
./ascii_art <image_path> [output_file.txt] [-c]
```

### Examples

**Basic Usage (Print to Console):**
```bash
./ascii_art photo.jpg
```

**Save to File:**
```bash
./ascii_art photo.jpg art.txt
```

**Color Output:**
```bash
./ascii_art photo.jpg -c
```

**View Help:**
```bash
./ascii_art -help
```

---

## Build Instructions

### Clone and build with CMake
```bash
git clone https://github.com/CiaranMccarthy1/ASCII_ART.git
cd ASCII_ART
mkdir build && cd build
cmake ..
cmake --build .
```

### Or compile directly with g++
```bash
g++ main.cpp -o ascii_art
```