#ifndef VERTEX_H
#define VERTEX_H

#include <iostream>


class Vertex {
public:
    Vertex(unsigned int n, std::string v = ""): number_(n), value_(v) {};
    ~Vertex() {};

    unsigned int number() {
        return number_;
    };

    std::string value() {
        return value_;
    }

    bool operator==(Vertex &other) {
        return (number_ == other.number_) && (value_ == other.value_);
    };

    bool operator!=(Vertex &other) {
        return !(*this == other);
    };

private:
    unsigned int number_;
    std::string value_;
};

#endif // VERTEX_H