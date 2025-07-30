#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <stdexcept>
#include <cstdint>

#pragma pack(push, 1)
struct BMPHeader {
    uint16_t file_type = 0x4D42;       // "BM"
    uint32_t file_size = 0;            // File size
    uint16_t reserved1 = 0;
    uint16_t reserved2 = 0;
    uint32_t offset_data = 54;         // Offset to pixel data

    uint32_t size = 40;                // DIB header size
    int32_t width = 0;                 // Image width
    int32_t height = 0;                // Image height
    uint16_t planes = 1;
    uint16_t bit_count = 24;           // 24 bits per pixel
    uint32_t compression = 0;          // No compression
    uint32_t size_image = 0;           // Pixel data size
    int32_t x_pixels_per_meter = 0;
    int32_t y_pixels_per_meter = 0;
    uint32_t colors_used = 0;
    uint32_t colors_important = 0;
};
#pragma pack(pop)

void generateRandomBMP(const std::string& filename, int width, int height) {
    if (width <= 0 || height <= 0 || width > 10000 || height > 10000) {
        throw std::invalid_argument("Invalid image dimensions (max 10000x10000)");
    }

    uint32_t row_stride = width * 3;
    uint32_t padding = (4 - (row_stride % 4)) % 4;
    row_stride += padding;

    BMPHeader header;
    header.width = width;
    header.height = height;
    header.size_image = row_stride * height;
    header.file_size = header.offset_data + header.size_image;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);

    std::vector<uint8_t> pixels(header.size_image);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int pos = y * row_stride + x * 3;
            uint8_t color = dis(gen) ? 255 : 0;
            pixels[pos] = color;     // B
            pixels[pos + 1] = color; // G
            pixels[pos + 2] = color; // R
        }
        for (uint32_t p = 0; p < padding; ++p) {
            pixels[y * row_stride + width * 3 + p] = 0;
        }
    }

    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file for writing: " + filename);
    }

    file.write(reinterpret_cast<const char*>(&header), sizeof(header));
    file.write(reinterpret_cast<const char*>(pixels.data()), pixels.size());
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <filename> <width> <height>\n";
        return 1;
    }

    try {
        std::string filename = argv[1];
        int width = std::stoi(argv[2]);
        int height = std::stoi(argv[3]);
        
        generateRandomBMP(filename, width, height);
        std::cout << "Generated " << filename 
                  << " with size " << width << "x" << height << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

#endif // GENERATOR_HPP