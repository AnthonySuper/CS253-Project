#ifndef ERRORS_HPP
#define ERRORS_HPP
#include <stdexcept>
#include <string>

class FileNotFoundError : public std::runtime_error {
    public:
        FileNotFoundError(std::string f);
};

class InvalidFormatError : public std::runtime_error {
    public:
        InvalidFormatError();
        InvalidFormatError(std::string msg);
};

#endif
