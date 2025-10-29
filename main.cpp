    #include <iostream>
    #include <fstream>
    #include <string>
    #include <algorithm>
    #include <limits>
    #include <cctype>

    #define STB_IMAGE_IMPLEMENTATION
    #include "stb_image.h"

    const std::string GRADIENT = "@%#*+=-:. ";

    int outputWidth;
    int outputHeight;

    std::string rgbToAscii(unsigned char r, unsigned char g, unsigned char b) {
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
        std::ofstream outputFile;
        bool toFile = false;
        bool colourOutput = false;

        for (int i = 1; i < argc; ++i) {  
            std::string arg = argv[i];
            if (arg == "-c") {
                colourOutput = true;
            } else if (arg.size() >= 4 && arg.substr(arg.size() - 4) == ".txt") {
                outputFileName = arg;
                toFile = true;
            } else {
                imagePath = arg;  
            }
        }

        if (toFile && colourOutput) {
            std::cout << "Note: Colour output disabled when saving to a file.\n";
            colourOutput = false;
}

        if (toFile) {
            outputFile.open(outputFileName.c_str());
            if (!outputFile) {
                std::cerr << "Failed to open output file: " << outputFileName << "\n";
                return 1;
        }
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

        std::ostream& out = toFile ? outputFile : std::cout;

        for (int y = 0; y < outputHeight; ++y) {
            int sy = std::min(height - 1, static_cast<int>((y + 0.5) * scaleY));
            for (int x = 0; x < outputWidth; ++x) {
                int sx = std::min(width - 1, static_cast<int>((x + 0.5) * scaleX));
                int idx = (sy * width + sx) * 3;

                unsigned char r = data[idx];
                unsigned char g = data[idx + 1];
                unsigned char b = data[idx + 2];

                float grey = 0.28f * r + 0.65f * g + 0.07f * b;
                int charIndex = static_cast<int>((grey / 255.0f) * (GRADIENT.size() - 1));
                charIndex = std::clamp(charIndex, 0, static_cast<int>(GRADIENT.size() - 1));

                if (colourOutput)
                    out << rgbToAscii(r, g, b) << GRADIENT[charIndex];
                else
                    out << GRADIENT[charIndex];
            }
            out << "\n";
        }

        if (colourOutput && !toFile)
            std::cout << "\033[0m";

        stbi_image_free(data);

    if (toFile) {
        outputFile.close();
        std::cout << "ASCII art saved to: " << outputFileName << "\n";
    }

        return 0;
    }
