#pragma once
#include <stdexcept>
#include <string>

class CommonMathException: public std::runtime_error {
public:
    CommonMathException(int code, const std::string& message)
        : std::runtime_error(message), error_code(code) {}
    
    int getErrorCode() const { return error_code; }

private:
    int error_code;
};

class VectorException : public CommonMathException {
public:
    explicit VectorException(int code, const std::string& message)
        : CommonMathException(code, "Vector error: " + message) {}
    
    explicit VectorException(int code, const char* message)
        : CommonMathException(code, message) {}
};

class MatrixException : public CommonMathException {
public:
    explicit MatrixException(int code, const std::string& message)
        : CommonMathException(code, "Matrix error: " + message) {}
    
    explicit MatrixException(int code, const char* message)
        : CommonMathException(code, message) {}
};
