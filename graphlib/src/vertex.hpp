#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <iostream>
#include <string>

#include "exception.hpp"

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

// Специализация std::hash для Vertex - ДОЛЖНА БЫТЬ В ТОЙ ЖЕ НАМЕСПЕЙС
namespace std {
    template<>
    struct hash<Vertex> {
        size_t operator()(const Vertex& v) const {
            // Простая хеш-функция
            return hash<unsigned int>{}(v.number()) ^ 
                   (hash<string>{}(v.value()) << 1);
        }
    };
}

#endif // VERTEX_HPP