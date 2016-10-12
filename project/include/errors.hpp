#ifndef ERRORS_HPP
#define ERRORS_HPP
#include <stdexcept>
#include <string>

/**
 * An error thrown when we cannot read from a file.
 */
class FileNotFoundError : public std::runtime_error {
    public:
        FileNotFoundError(std::string filename) :
            runtime_error{"File not found " + filename}
        {}
};

/**
 * An error thrown when a file has an invalid format of some kind
 */
class InvalidFormatError : public std::runtime_error {
    public:
        InvalidFormatError() : 
            runtime_error{"Input had invalid format!"}
        {}
        InvalidFormatError(std::string msg) : 
            runtime_error{msg}
        {}
};

/**
 * An error thrown when a bad number is read.
 * Really should be a sub-class of InvalidFormatError, at some point.
 */
class BadNumberError : public std::runtime_error {
    public:
        BadNumberError(int i) :
            runtime_error("Recieved unexpected number " + std::to_string(i))
    {}
};

#endif
