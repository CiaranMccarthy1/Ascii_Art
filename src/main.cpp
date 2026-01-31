#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <limits>
#include <cctype>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const std::string GRADIENT = "@%#*+=-:. ";

// Helper to generate ANSI color code
std::string rgbToAnsi(unsigned char r, unsigned char g, unsigned char b) {
    return "\033[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
}

int getValidInput(const std::string& prompt) {
    int value;
    std::cout << prompt;
    while (!(std::cin >> value) || (value < 1)) {
        std::cout << "Invalid input. Please enter a positive integer: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return value;
}

bool fixAspectRatio(const std::string& prompt) {
    char choice;
    std::cout << prompt << " (y/n): ";
    while (true) {
        if (std::cin >> choice) {
            choice = std::tolower(choice);
            if (choice == 'y') return true;
            if (choice == 'n') return false;
        }
        std::cout << "Invalid input. Please enter 'y' or 'n': ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <image_path> [output_file.txt] [-c]\n";
        std::cerr << "Example: " << argv[0] << " photo.jpg -c\n";
        return 1;
    }

    std::string imagePath;
    std::string outputFileName;
    bool toFile = false;
    bool colourOutput = false;

    // improved argument parsing
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-c") {
            colourOutput = true;
        } else if (arg == "-help" || arg == "--help") {
            std::cout << "Usage: " << argv[0] << " <image_path> [output_file.txt] [-c]\n";
            std::cout << "Options:\n";
            std::cout << "  <image_path>      Path to the input image file (jpg, png, etc.)\n";
            std::cout << "  [output_file.txt] Optional path to save the ASCII art to a text file.\n";
            std::cout << "  -c                Enable colored output (ANSI codes).\n";
            std::cout << "  -help, --help     Show this help message.\n";
            return 0;
        } else if (arg.size() >= 4 && arg.substr(arg.size() - 4) == ".txt") {
            outputFileName = arg;
            toFile = true;
        } else { // Assume it's the image path if not a flag or txt file
            imagePath = arg;
        }
    }

    if (imagePath.empty()) {
         std::cerr << "Error: No image path provided.\n";
         return 1;
    }

    // Load image first to fail early if invalid
    int width, height, channels;
    unsigned char* data = stbi_load(imagePath.c_str(), &width, &height, &channels, 3);
    if (!data) {
        std::cerr << "Failed to load image: " << imagePath << "\n";
        return 1;
    }

    // Now get user input
    int outputWidth = getValidInput("Enter output width: ");
    int outputHeight = getValidInput("Enter output height: ");
    bool fixAspect = fixAspectRatio("Fix aspect ratio for better proportions");

    if (fixAspect) {
        outputHeight = static_cast<int>(outputHeight * 0.45);
        if (outputHeight < 1) outputHeight = 1;
    }

    std::ofstream outputFile;
    if (toFile) {
        outputFile.open(outputFileName.c_str());
        if (!outputFile) {
            std::cerr << "Failed to open output file: " << outputFileName << "\n";
            stbi_image_free(data);
            return 1;
        }
    }

    double scaleX = static_cast<double>(width) / outputWidth;
    double scaleY = static_cast<double>(height) / outputHeight;

    std::string consoleBuffer;
    std::string fileBuffer;

    for (int y = 0; y < outputHeight; ++y) {
        consoleBuffer.clear();
        fileBuffer.clear();
        
        for (int x = 0; x < outputWidth; ++x) {
            // Box Sampling Implementation (Averaging)
            int startX = static_cast<int>(x * scaleX);
            int endX = static_cast<int>((x + 1) * scaleX);
            int startY = static_cast<int>(y * scaleY);
            int endY = static_cast<int>((y + 1) * scaleY);

            // Clamp to image boundaries and ensure non-zero range
            startX = std::clamp(startX, 0, width - 1);
            endX = std::max(startX + 1, std::min(endX, width));
            startY = std::clamp(startY, 0, height - 1);
            endY = std::max(startY + 1, std::min(endY, height));
            
            long rSum = 0, gSum = 0, bSum = 0;
            int count = 0;

            for (int sy = startY; sy < endY; ++sy) {
                for (int sx = startX; sx < endX; ++sx) {
                    int idx = (sy * width + sx) * 3;
                    rSum += data[idx];
                    gSum += data[idx + 1];
                    bSum += data[idx + 2];
                    count++;
                }
            }
            
            // Fallback for edge cases (shouldn't really hit if logic is sound)
            if (count == 0) { 
                 int idx = (startY * width + startX) * 3;
                 rSum = data[idx]; gSum = data[idx+1]; bSum = data[idx+2];
                 count = 1;
            }

            unsigned char r = static_cast<unsigned char>(rSum / count);
            unsigned char g = static_cast<unsigned char>(gSum / count);
            unsigned char b = static_cast<unsigned char>(bSum / count);

            float grey = 0.299f * r + 0.587f * g + 0.114f * b; 
            int charIndex = static_cast<int>((grey / 255.0f) * (GRADIENT.size() - 1));
            charIndex = std::clamp(charIndex, 0, static_cast<int>(GRADIENT.size() - 1));

            char asciiChar = GRADIENT[charIndex];

            if (colourOutput) {
                consoleBuffer += rgbToAnsi(r, g, b);
            }
            consoleBuffer += asciiChar;
            
            if (toFile) {
                fileBuffer += asciiChar;
            }
        }
        
        // End of line
        if (colourOutput) {
             consoleBuffer += "\033[0m\n"; // clear color at EOL so it doesn't bleed if resizing
        } else {
             consoleBuffer += "\n";
        }
        
        std::cout << consoleBuffer;
        
        if (toFile) {
            fileBuffer += "\n";
            outputFile << fileBuffer;
        }
    }
    
    // Explicit reset at the very end just in case
    if (colourOutput)
        std::cout << "\033[0m";

    stbi_image_free(data);

    if (toFile) {
        outputFile.close();
        
        // Fix indentation here
        std::cout << "ASCII art saved to: " << outputFileName << "\n";
    }

    return 0;
}
