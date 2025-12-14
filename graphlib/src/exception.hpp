#pragma once
#include <stdexcept>
#include <string>

class GraphException: public std::runtime_error {
public:
    GraphException(int code, const std::string& message)
        : std::runtime_error(message), error_code(code) {}
    
    int getErrorCode() const { return error_code; }

private:
    int error_code;
};
