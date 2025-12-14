#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <iostream>
#include <string>

class Vertex {
public:
    Vertex(unsigned int n, std::string v = ""): number_(n), value_(v) {};
    ~Vertex() = default;

    // Константные методы доступа
    unsigned int number() const {
        return number_;
    };

    std::string value() const {
        return value_;
    }

    // Константные операторы сравнения
    bool operator==(const Vertex &other) const {
        return (number_ == other.number_) && (value_ == other.value_);
    };

    bool operator!=(const Vertex &other) const {
        return !(*this == other);
    };

private:
    unsigned int number_;
    std::string value_;
};

#endif // VERTEX_HPP