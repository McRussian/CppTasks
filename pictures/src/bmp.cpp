#include "bmp.h"
#include <iostream>
#include <fstream>
#include <stdexcept>

BMPReader::BMPReader(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    // Read header
    file.read(reinterpret_cast<char*>(&header), sizeof(header));
    
    // Verify BMP format
    if (header.file_type != 0x4D42) {
        throw std::runtime_error("Not a BMP file");
    }
    
    // Check bit depth (only 24 or 32 bits supported)
    if (header.bit_count != 24 && header.bit_count != 32) {
        throw std::runtime_error("Only 24-bit and 32-bit BMP files are supported");
    }
    
    // Check compression (must be uncompressed)
    if (header.compression != 0) {
        throw std::runtime_error("Only uncompressed BMP files are supported");
    }
    
    // Read pixel data
    file.seekg(header.offset_data, std::ios::beg);
    data.resize(header.width * header.height * (header.bit_count / 8));
    file.read(reinterpret_cast<char*>(data.data()), data.size());
    
    // Validate that only black and white colors are present
    checkColors();
}

void BMPReader::checkColors() const {
    size_t bytes_per_pixel = header.bit_count / 8;
    
    for (size_t i = 0; i < data.size(); i += bytes_per_pixel) {
        uint8_t blue = data[i];
        uint8_t green = data[i + 1];
        uint8_t red = data[i + 2];
        
        bool is_black = (red == 0) && (green == 0) && (blue == 0);
        bool is_white = (red == 255) && (green == 255) && (blue == 255);
        
        if (!is_black && !is_white) {
            throw std::runtime_error("Image contains colors other than black and white");
        }
    }
}

void BMPReader::printToConsole(char black_char, char white_char) const {
    size_t bytes_per_pixel = header.bit_count / 8;
    size_t row_padding = (4 - (header.width * bytes_per_pixel) % 4) % 4;
    
    // BMP stores pixels from bottom to top
    for (int y = header.height - 1; y >= 0; --y) {
        for (int x = 0; x < header.width; ++x) {
            size_t index = (y * header.width + x) * bytes_per_pixel;
            
            uint8_t blue = data[index];
            uint8_t green = data[index + 1];
            uint8_t red = data[index + 2];
            
            if (red == 0 && green == 0 && blue == 0) {
                std::cout << black_char;
            } else {
                std::cout << white_char;
            }
        }
        std::cout << '\n';
    }
}
