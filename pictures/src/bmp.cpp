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
    
    try {
        // Validate that only black and white colors are present
    checkColors();
    } catch (...) {
        // Если возникло исключение, освобождаем ресурсы
        data.clear();
        throw; // Перебрасываем исключение дальше
    }
}

BMPReader::~BMPReader() {
    // Вектор data очистится автоматически
    // Дополнительных ресурсов для освобождения нет
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

void BMPReader::saveToFile(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file for writing: " + filename);
    }

    file.write(reinterpret_cast<const char*>(&header), sizeof(header));
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}

void BMPReader::validatePoint(int x, int y) const {
    if (x < 0 || x >= header.width || y < 0 || y >= header.height) {
        throw std::out_of_range("Point coordinates are out of image bounds");
    }
}

void BMPReader::drawLine(int x1, int y1, int x2, int y2) {
    validatePoint(x1, y1);
    validatePoint(x2, y2);

    size_t bytes_per_pixel = header.bit_count / 8;
    size_t row_stride = header.width * bytes_per_pixel;
    row_stride += (4 - (row_stride % 4)) % 4;
    
    // Алгоритм Брезенхэма
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2;

    while (true) {
        size_t index = ((header.height - 1 - y1) * row_stride) + x1 * bytes_per_pixel;
        data[index] = 0;     // B
        data[index + 1] = 0; // G
        data[index + 2] = 0; // R

        if (x1 == x2 && y1 == y2) break;
        
        int e2 = err;
        if (e2 > -dx) { err -= dy; x1 += sx; }
        if (e2 < dy) { err += dx; y1 += sy; }
    }
}

std::pair<bool, std::pair<int, int>> BMPReader::checkLinesIntersection(
    int line1_x1, int line1_y1, int line1_x2, int line1_y2,
    int line2_x1, int line2_y1, int line2_x2, int line2_y2) const {
    
    // Проверяем что первая линия либо горизонтальная, либо вертикальная
    bool line1_vertical = (line1_x1 == line1_x2);
    bool line1_horizontal = (line1_y1 == line1_y2);
    
    // Проверяем что вторая линия либо горизонтальная, либо вертикальная
    bool line2_vertical = (line2_x1 == line2_x2);
    bool line2_horizontal = (line2_y1 == line2_y2);
    
    // Линии должны быть перпендикулярны
    if (!((line1_vertical && line2_horizontal) || (line1_horizontal && line2_vertical))) {
        return {false, {0, 0}};
    }
    
    // Если первая линия вертикальная, а вторая горизонтальная
    if (line1_vertical && line2_horizontal) {
        // Проверяем пересечение
        if (line2_x1 <= line1_x1 && line1_x1 <= line2_x2 &&
            line1_y1 <= line2_y1 && line2_y1 <= line1_y2) {
            return {true, {line1_x1, line2_y1}};
        }
    }
    // Если первая линия горизонтальная, а вторая вертикальная
    else if (line1_horizontal && line2_vertical) {
        // Проверяем пересечение
        if (line1_x1 <= line2_x1 && line2_x1 <= line1_x2 &&
            line2_y1 <= line1_y1 && line1_y1 <= line2_y2) {
            return {true, {line2_x1, line1_y1}};
        }
    }
    
    return {false, {0, 0}};
}

void BMPReader::drawCross(int line1_x1, int line1_y1, int line1_x2, int line1_y2,
                         int line2_x1, int line2_y1, int line2_x2, int line2_y2) {
    // Проверяем что линии перпендикулярны и пересекаются
    auto [intersects, intersection] = checkLinesIntersection(
        line1_x1, line1_y1, line1_x2, line1_y2,
        line2_x1, line2_y1, line2_x2, line2_y2);
    
    if (!intersects) {
        throw std::invalid_argument(
            "Lines must be perpendicular (one vertical and one horizontal) and intersect");
    }
    
    // Проверяем координаты
    validatePoint(line1_x1, line1_y1);
    validatePoint(line1_x2, line1_y2);
    validatePoint(line2_x1, line2_y1);
    validatePoint(line2_x2, line2_y2);
    
    // Рисуем обе линии
    drawLine(line1_x1, line1_y1, line1_x2, line1_y2);
    drawLine(line2_x1, line2_y1, line2_x2, line2_y2);
}