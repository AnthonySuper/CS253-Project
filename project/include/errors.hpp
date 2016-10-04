#ifndef ERRORS_HPP
#define ERRORS_HPP
#include <stdexcept>
#include <string>

class FileNotFoundError : public std::runtime_error {
    public:
        FileNotFoundError(std::string filename) :
            runtime_error{"File not found " + filename}
        {}
};

class InvalidFormatError : public std::runtime_error {
    public:
        InvalidFormatError() : 
            runtime_error{"Input had invalid format!"}
        {}
        InvalidFormatError(std::string msg) : 
            runtime_error{msg}
        {}
};

class BadNumberError : public std::runtime_error {
    public:
        BadNumberError(int i) :
            runtime_error("Recieved unexpected number " + std::to_string(i))
    {}
};

#endif
