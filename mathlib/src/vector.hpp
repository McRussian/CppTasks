#pragma once
#include <iostream>
#include <vector>
#include <initializer_list>
#include <concepts>
#include <type_traits>

#include "exception.hpp"


// Вспомогательный концепт для проверки стандартных арифметических типов
template <typename T>
concept StandardArithmetic = std::is_arithmetic_v<T>;

template <typename T>
concept VectorElement = requires(T a, T b, int i, float f, double d) {
    // Базовые операции между элементами одного типа
    {a + b} -> std::convertible_to<T>;
    {a - b} -> std::convertible_to<T>;
    {a * b} -> std::convertible_to<T>;
    {a += b} -> std::same_as<T&>;
    {a -= b} -> std::same_as<T&>;
    { -a } -> std::convertible_to<T>;
    {T{0}};
    
    // Умножение на стандартные арифметические типы
    {a * i} -> std::convertible_to<T>;
    {i * a} -> std::convertible_to<T>;
    {a * f} -> std::convertible_to<T>;
    {f * a} -> std::convertible_to<T>;
    {a * d} -> std::convertible_to<T>;
    {d * a} -> std::convertible_to<T>;
    
    // Сложение с стандартными арифметическими типами (если нужно)
    {a + i} -> std::convertible_to<T>;
    {i + a} -> std::convertible_to<T>;
    {a + f} -> std::convertible_to<T>;
    {f + a} -> std::convertible_to<T>;
    {a + d} -> std::convertible_to<T>;
    {d + a} -> std::convertible_to<T>;
    
    // Для вывода в поток
    { std::declval<std::ostream&>() << a } -> std::same_as<std::ostream&>;
};


template <VectorElement T>
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

    Vector(const std::vector<T> vect) : size_(vect.size()) {
        data_ = new T[size_];
        if (data_ == nullptr)
            throw VectorException(1, "Failed to allocate memory for vector");
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = vect[i];
        }
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

    // Умножение на скаляр (любого арифметического типа)
    template <StandardArithmetic U>
    Vector operator*(U scalar) const {
        Vector result(size_);
        for (size_t i = 0; i < size_; ++i) {
            result[i] = data_[i] * static_cast<T>(scalar);
        }
        return result;
    }

    // Дружественная функция для умножения скаляра на вектор
    template <StandardArithmetic U, VectorElement V>
    friend Vector<V> operator*(U scalar, const Vector<V>& vec) {
        return vec * scalar;
    }

    // Сложение с скаляром (если нужно)
    template <StandardArithmetic U>
    Vector operator+(U scalar) const {
        Vector result(size_);
        for (size_t i = 0; i < size_; ++i) {
            result[i] = data_[i] + static_cast<T>(scalar);
        }
        return result;
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

    // Метод для взятия срезки вектора
    Vector<T> slice(int start, int end, int step = 1) const {
        // Проверка корректности шага
        if (step == 0) {
            throw VectorException(20, "Step cannot be zero");
        }

        // Обработка отрицательных индексов (отсчет с конца)
        int actual_start = (start >= 0) ? start : size_ + start;
        int actual_end = (end >= 0) ? end : size_ + end;

        // Проверка границ
        if (actual_start >= size_ || actual_end >= size_) {
            throw VectorException(20, "Slice indices out of range");
        }

        // Определение направления срезки
        bool forward = (step > 0);
        bool valid_range = forward ? (actual_start <= actual_end) : (actual_start >= actual_end);

        if (!valid_range) {
            throw VectorException(20, "Invalid slice range for given step direction");
        }

        // Вычисление размера результирующего вектора
        size_t new_size = (abs(actual_end - actual_start) + abs(step)) / abs(step);
        Vector<T> result(new_size);

        // Заполнение результирующего вектора
        if (forward) {
            for (size_t i = 0, j = actual_start; i < new_size && j <= actual_end; ++i, j += step) {
                result[i] = data_[j];
            }
        } else {
            for (size_t i = 0, j = actual_start; i < new_size && j >= actual_end; ++i, j += step) {
                result[i] = data_[j];
            }
        }

        return result;
    }

private:
    size_t size_;
    T* data_;
};
