#include <iostream>
#include "src/vector.hpp"


int main() {
    Vector<double> vec(5);
    std::cout << "Size of Vector: " << vec.size() << std::endl;
    for(unsigned int i = 0; i < vec.size(); i++) {
        std::cout << "Vec[" << i << "] = " << vec.at(i) << std::endl;
    }

}