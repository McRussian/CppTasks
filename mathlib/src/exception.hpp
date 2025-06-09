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


class VectorException: CommonMathException
{

};

class MatrixException: CommonMathException
{

};
