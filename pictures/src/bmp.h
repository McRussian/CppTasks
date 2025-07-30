#ifndef BMP_READER_H
#define BMP_READER_H

#include <fstream>
#include <vector>
#include <stdexcept>
#include <cstdint>
#include <string>

#pragma pack(push, 1)
struct BMPHeader {
    uint16_t file_type{0x4D42};          // File type always BM which is 0x4D42
    uint32_t file_size{0};               // Size of the file (in bytes)
    uint16_t reserved1{0};               // Reserved, always 0
    uint16_t reserved2{0};               // Reserved, always 0
    uint32_t offset_data{0};             // Start position of pixel data (bytes from the beginning of the file)

    uint32_t size{0};                    // Size of this header (in bytes)
    int32_t width{0};                    // width of bitmap in pixels
    int32_t height{0};                   // height of bitmap in pixels
    uint16_t planes{1};                  // No. of planes for the target device, this is always 1
    uint16_t bit_count{0};               // No. of bits per pixel
    uint32_t compression{0};             // 0 or 3 - uncompressed. THIS PROGRAM CONSIDERS ONLY UNCOMPRESSED BMP images
    uint32_t size_image{0};              // 0 - for uncompressed images
    int32_t x_pixels_per_meter{0};
    int32_t y_pixels_per_meter{0};
    uint32_t colors_used{0};             // No. color indexes in the color table. Use 0 for the max number of colors allowed by bit_count
    uint32_t colors_important{0};        // No. of colors used for displaying the bitmap. If 0 all colors are required
};
#pragma pack(pop)

class BMPReader {
public:
    explicit BMPReader(const std::string& filename);
    ~BMPReader(); // Деструктор
    
    // Запрещаем копирование и присваивание
    BMPReader(const BMPReader&) = delete;
    BMPReader& operator=(const BMPReader&) = delete;
    
    void printToConsole(char black_char = '@', char white_char = ' ') const;
    // Рисует крест из двух линий (должны быть одна вертикальная и одна горизонтальная)
    void drawCross(int line1_x1, int line1_y1, int line1_x2, int line1_y2,
                  int line2_x1, int line2_y1, int line2_x2, int line2_y2);
    
    void saveToFile(const std::string& filename) const;
    
private:
    void drawLine(int x1, int y1, int x2, int y2);
    void checkColors() const;
    void validatePoint(int x, int y) const;
    std::pair<bool, std::pair<int, int>> checkLinesIntersection(
        int line1_x1, int line1_y1, int line1_x2, int line1_y2,
        int line2_x1, int line2_y1, int line2_x2, int line2_y2) const;
    
    BMPHeader header;
    std::vector<uint8_t> data;
};

#endif // BMP_READER_H