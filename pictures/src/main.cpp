#include "bmp.h"
#include <iostream>
#include <string>

int main() {
    try {
        std::string input_filename;
        std::cout << "Enter input BMP file name: ";
        std::cin >> input_filename;
        
        BMPReader reader(input_filename);
        
        // Вывод исходного изображения
        std::cout << "\nOriginal image:\n";
        reader.printToConsole('#', ' ');
        
        // Получаем параметры для линий
        int line1_x1, line1_y1, line1_x2, line1_y2;
        int line2_x1, line2_y1, line2_x2, line2_y2;
        
        std::cout << "\nEnter first line points (x1 y1 x2 y2): ";
        std::cin >> line1_x1 >> line1_y1 >> line1_x2 >> line1_y2;
        
        std::cout << "Enter second line points (x1 y1 x2 y2): ";
        std::cin >> line2_x1 >> line2_y1 >> line2_x2 >> line2_y2;
        
        // Рисуем крест
        reader.drawCross(line1_x1, line1_y1, line1_x2, line1_y2,
                        line2_x1, line2_y1, line2_x2, line2_y2);
        
        // Вывод модифицированного изображения
        std::cout << "\nImage with cross:\n";
        reader.printToConsole('#', ' ');
        
        // Сохранение результата
        std::string output_filename;
        std::cout << "\nEnter output BMP file name: ";
        std::cin >> output_filename;
        
        reader.saveToFile(output_filename);
        std::cout << "Image saved to " << output_filename << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}