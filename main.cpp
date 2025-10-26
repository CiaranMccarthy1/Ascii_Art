#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <limits>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const std::string GRADIENT = "@%#*+=-:. ";

int outputWidth;
int outputHeight;

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
        std::cerr << "Usage: " << argv[0] << " <image_path> [output_file.txt]\n";
        return 1;
    }

    std::string imagePath = argv[1];
    std::ofstream outFile;
    bool toFile = false;
    if (argc >= 3) {
        outFile.open(argv[2]);
        if (!outFile) {
            std::cerr << "Failed to open output file: " << argv[2] << "\n";
            return 1;
        }
        toFile = true;
    }

    outputWidth = getValidInput("Enter output width: ");
    outputHeight = getValidInput("Enter output height: ");
    bool fixAspect = fixAspectRatio("Fix aspect ratio for better proportions");

    int width, height, channels;
    unsigned char* data = stbi_load(imagePath.c_str(), &width, &height, &channels, 3);
    if (!data) {
        std::cerr << "Failed to load image: " << imagePath << "\n";
        return 1;
    }

    if (fixAspect) outputHeight = static_cast<int>(outputHeight * 0.55);

    double scaleX = static_cast<double>(width) / outputWidth;
    double scaleY = static_cast<double>(height) / outputHeight;

    std::ostream& out = toFile ? outFile : std::cout;

    for (int y = 0; y < outputHeight; ++y) {
        int sy = std::min(height - 1, static_cast<int>((y + 0.5) * scaleY));
        for (int x = 0; x < outputWidth; ++x) {
            int sx = std::min(width - 1, static_cast<int>((x + 0.5) * scaleX));
            int idx = (sy * width + sx) * 3;

            unsigned char r = data[idx + 0];
            unsigned char g = data[idx + 1];
            unsigned char b = data[idx + 2];

            float grey = 0.2126f * r + 0.7152f * g + 0.0722f * b;
            int charIndex = static_cast<int>((grey / 255.0f) * (GRADIENT.size() - 1));
            charIndex = std::clamp(charIndex, 0, static_cast<int>(GRADIENT.size() - 1));

            std::cout << GRADIENT[charIndex];
            if (toFile) outFile << GRADIENT[charIndex];
        }
        std::cout << "\n";
        if (toFile) outFile << "\n";
    }

    stbi_image_free(data);
    if (toFile) {
        outFile.close();
        std::cout << "ASCII art saved to: " << argv[2] << "\n";
    }

    return 0;
}
