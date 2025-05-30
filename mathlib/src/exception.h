#include <exception>
#include <string>

class CommonMathException: std::exception
{
public: 
    CommonMathException(unsigned int code, std::string message): 
        code_(code),
        message_{message}
    {}

    std::string getMessage() const {return message_;}
    unsigned int getCode() const {return code_;}
private:
    unsigned int code_;
    std::string message_;
};


class VectorException: CommonMathException
{

};

class MatrixException: CommonMathException
{

};
