#include "bmp.h"
#include <iostream>
#include <string>

int main() {
    try {
        std::string filename;
        std::cout << "Enter input BMP file name: ";
        std::cin >> filename;
        
        BMPReader reader(filename);
        reader.printToConsole('#', ' ');
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}