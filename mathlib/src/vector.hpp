#pragma once
#include <iostream>
#include <initializer_list>

#include "exception.hpp"

template <typename T>
class Vector {
public:
    explicit Vector(size_t size): size_(size) {
        try {
            data_ = new T[size_]();
        } catch (const std::bad_alloc&) {
            throw VectorException(1, "Failed to allocate memory for vector");
        }
    }

    Vector(size_t size, T value): size_(size) {
        try {
            data_ = new T[size_];
        } catch (const std::bad_alloc&) {
            throw VectorException(1, "Failed to allocate memory for vector");
        }
        for (size_t i = 0; i < size_; i++)
            data_[i] = value;
    }

    // Конструктор с инициализацией списком
    Vector(std::initializer_list<T> init) : size_(init.size()) {
        try {
            data_ = new T[size_];
        } catch (const std::bad_alloc&) {
            throw VectorException(1, "Failed to allocate memory for vector");
        }
        size_t i = 0;
        for (const auto& item : init) {
            data_[i++] = item;
        }
    }

    // Конструктор копирования
    Vector(const Vector& other) : size_(other.size_) {
        data_ = new T[size_];
        if (data_ == nullptr)
            throw VectorException(1, "Failed to allocate memory for vector");
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }

    // Конструктор перемещения
    Vector(Vector&& other) noexcept : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;
        other.size_ = 0;
    }

    // Деструктор
    ~Vector() {
        delete[] data_;
    }

    // Оператор присваивания копированием
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = new T[size_];
            if (data_ == nullptr)
                throw VectorException(1, "Failed to allocate memory for vector");
            for (size_t i = 0; i < size_; ++i) {
                data_[i] = other.data_[i];
            }
        }
        return *this;
    }

    // Оператор присваивания перемещением
    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            other.data_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    // Метод для получения размера вектора
    size_t size() const {
        return size_;
    }

    // Метод доступа к элементу с проверкой границ
    T& at(size_t index) {
        if (index >= size_) {
            throw VectorException(5, "Index out of range");
        }
        return data_[index];
    }

    // Константный метод доступа к элементу с проверкой границ
    const T& at(size_t index) const {
        if (index >= size_) {
            throw VectorException(5, "Index out of range");
        }
        return data_[index];
    }

    // Оператор индексирования
    T& operator[](size_t index) {
        if (index >= size_) {
            throw VectorException(5, "Index out of range");
        }
        return data_[index];
    }

    // Константный оператор индексирования
    const T& operator[](size_t index) const {
        return data_[index];
    }

    // Оператор сравнения
    bool operator==(const Vector<T>& other) const {
        if (size_ != other.size_) {
            throw VectorException(10, "Vectors must be of the same size");
        }
        return std::equal(data_, data_ + size_, other.data_);
    }

    // Оператор неравенства
    bool operator!=(const Vector<T>& other) const {
        return !(*this == other);
    }

    // Оператор сложения векторов
    Vector operator+(const Vector& other) const {
        if (size_ != other.size_) {
            throw VectorException(10, "Vectors must be of the same size");
        }
        Vector result(size_);
        for (size_t i = 0; i < size_; ++i) {
            result[i] = data_[i] + other.data_[i];
        }
        return result;
    }

    // Оператор вычитания векторов
    Vector operator-(const Vector& other) const {
        if (size_ != other.size_) {
            throw VectorException(10, "Vectors must be of the same size");
        }
        Vector result(size_);
        for (size_t i = 0; i < size_; ++i) {
            result[i] = data_[i] - other.data_[i];
        }
        return result;
    }

    // Оператор умножения векторов (скалярное произведение)
    T operator*(const Vector& other) const {
        if (size_ != other.size_) {
            throw VectorException(10, "Vectors must be of the same size");
        }
        T result = T();
        for (size_t i = 0; i < size_; ++i) {
            result += data_[i] * other.data_[i];
        }
        return result;
    }

    // Оператор умножения на скаляр
    Vector operator*(T scalar) const {
        Vector result(size_);
        for (size_t i = 0; i < size_; ++i) {
            result[i] = data_[i] * scalar;
        }
        return result;
    }

    // Дружественная функция для умножения скаляра на вектор
    friend Vector operator*(T scalar, const Vector& vec) {
        return vec * scalar;
    }

    // Оператор вывода в поток
    friend std::ostream& operator<<(std::ostream& os, const Vector& vec) {
        os << "[";
        for (size_t i = 0; i < vec.size_; ++i) {
            os << vec.data_[i];
            if (i != vec.size_ - 1) {
                os << ", ";
            }
        }
        os << "]";
        return os;
    }

private:
    size_t size_;
    T* data_;
};
