#include <iostream>
#include <initializer_list>
#include <concepts>
#include <vector>
#include <memory>

#include "exception.hpp"

template <typename T>
concept MatrixElement = requires(T a, T b, int i, float f, double d) {
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
    
    // Для вывода в поток
    { std::declval<std::ostream&>() << a } -> std::same_as<std::ostream&>;
};

template <MatrixElement T>
class Matrix {
private:
    std::unique_ptr<T[]> data_;
    size_t rows_;
    size_t cols_;

    // Проверка индексов
    void check_index_(size_t row, size_t col) const {
        if (row >= rows_ || col >= cols_) {
            throw MatrixException(13, "Matrix index out of range");
        }
    }

    void alloc_memory_(size_t rows, size_t cols) {
        try {
            data_ = std::make_unique<T[]>(rows * cols);
        } catch (const std::bad_alloc&) {
            throw MatrixException(1, "Failed to allocate memory for vector");
        }

    }

public:
    // Конструктор с указанием размеров
    Matrix(size_t rows, size_t cols) : rows_(rows), cols_(cols) {
        alloc_memory_(rows_, cols_);
        for (size_t i = 0; i < rows * cols; ++i) {
            data_[i] = T{};
        }
    }

    // Конструктор с указанием размеров и указанного значения
    Matrix(size_t rows, size_t cols, T value) : rows_(rows), cols_(cols) {
        alloc_memory_(rows_, cols_);
        for (size_t i = 0; i < rows * cols; ++i) {
            data_[i] = value;
        }
    }

    // Конструктор с инициализацией списком списков
    Matrix(std::initializer_list<std::initializer_list<T>> init) {
        rows_ = init.size();
        if (rows_ == 0) {
            cols_ = 0;
            return;
        }
        cols_ = init.begin()->size();
        alloc_memory_(rows_, cols_);
        
        size_t i = 0;
        for (const auto& row_list : init) {
            if (row_list.size() != cols_) {
                throw MatrixException(1, "All rows must have the same number of columns");
            }
            for (const auto& val : row_list) {
                data_[i++] = val;
            }
        }
    }

    // Конструктор копирования
    Matrix(const Matrix& other) : rows_(other.rows_), cols_(other.cols_) {
        alloc_memory_(rows_, cols_);
        for (size_t i = 0; i < rows_ * cols_; ++i) {
            data_[i] = other.data_[i];
        }
    }

    // Конструктор перемещения
    Matrix(Matrix&& other) noexcept : data_(std::move(other.data_)), rows_(other.rows_), cols_(other.cols_) {
        other.rows_ = 0;
        other.cols_ = 0;
    }

    // Оператор присваивания
    Matrix& operator=(const Matrix& other) {
        if (this != &other) {
            rows_ = other.rows_;
            cols_ = other.cols_;
            alloc_memory_(rows_, cols_);
            for (size_t i = 0; i < rows_ * cols_; ++i) {
                data_[i] = other.data_[i];
            }
        }
        return *this;
    }

    // Оператор перемещения
    Matrix& operator=(Matrix&& other) noexcept {
        if (this != &other) {
            rows_ = other.rows_;
            cols_ = other.cols_;
            data_ = std::move(other.data_);
            other.rows_ = 0;
            other.cols_ = 0;
        }
        return *this;
    }

    // Доступ к элементам с проверкой границ
    T& at(size_t row, size_t col) {
        check_index_(row, col);
        return data_[row * cols_ + col];
    }

    const T& at(size_t row, size_t col) const {
        check_index_(row, col);
        return data_[row * cols_ + col];
    }

    // Двойная индексация [row][col]
    class RowProxy {
    private:
        T* row_data_;
        size_t cols_;
    public:
        RowProxy(T* data, size_t cols) : row_data_(data), cols_(cols) {}
        T& operator[](size_t col) {
            if (col >= cols_) {
                throw MatrixException(5, "Column index out of range");
            }
            return row_data_[col];
        }
        const T& operator[](size_t col) const {
            if (col >= cols_) {
                throw MatrixException(5, "Column index out of range");
            }
            return row_data_[col];
        }
    };

    RowProxy operator[](size_t row) {
        if (row >= rows_) {
            throw MatrixException(10, "Row index out of range");
        }
        return RowProxy(&data_[row * cols_], cols_);
    }

    const RowProxy operator[](size_t row) const {
        if (row >= rows_) {
            throw MatrixException(10, "Row index out of range");
        }
        return RowProxy(const_cast<T*>(&data_[row * cols_]), cols_);
    }

    // Получение размеров матрицы
    size_t get_rows() const { return rows_; }
    size_t get_cols() const { return cols_; }

    // Операция сложения матриц
    Matrix operator+(const Matrix& other) const {
        if (rows_ != other.rows_ || cols_ != other.cols_) {
            throw MatrixException( 15, "Matrix dimensions must agree");
        }
        Matrix result(rows_, cols_);
        for (size_t i = 0; i < rows_ * cols_; ++i) {
            result.data_[i] = data_[i] + other.data_[i];
        }
        return result;
    }

    // Операция вычитания матриц
    Matrix operator-(const Matrix& other) const {
        if (rows_ != other.rows_ || cols_ != other.cols_) {
            throw MatrixException( 15, "Matrix dimensions must agree");
        }
        Matrix result(rows_, cols_);
        for (size_t i = 0; i < rows_ * cols_; ++i) {
            result.data_[i] = data_[i] - other.data_[i];
        }
        return result;
    }

    // Операция умножения матриц
    Matrix operator*(const Matrix& other) const {
        if (cols_ != other.rows_) {
            throw MatrixException(15, "Number of columns in first matrix must equal number of rows in second matrix");
        }
        Matrix result(rows_, other.cols_);
        for (size_t i = 0; i < rows_; ++i) {
            for (size_t j = 0; j < other.cols_; ++j) {
                T sum = T{};
                for (size_t k = 0; k < cols_; ++k) {
                    sum += data_[i * cols_ + k] * other.data_[k * other.cols_ + j];
                }
                result.data_[i * other.cols_ + j] = sum;
            }
        }
        return result;
    }

    // Умножение на скаляр
    Matrix operator*(T scalar) const {
        Matrix result(rows_, cols_);
        for (size_t i = 0; i < rows_ * cols_; ++i) {
            result.data_[i] = data_[i] * scalar;
        }
        return result;
    }

    // Дружественная функция для умножения скаляра на матрицу
    friend Matrix operator*(T scalar, const Matrix& mat) {
        return mat * scalar;
    }

    // Вывод матрицы в поток
    friend std::ostream& operator<<(std::ostream& os, const Matrix& mat) {
        for (size_t i = 0; i < mat.rows_; ++i) {
            os << "[";
            for (size_t j = 0; j < mat.cols_; ++j) {
                os << mat.data_[i * mat.cols_ + j];
                if (j != mat.cols_ - 1) {
                    os << ", ";
                }
            }
            os << "]";
            if (i != mat.rows_ - 1) {
                os << "\n";
            }
        }
        return os;
    }
};
