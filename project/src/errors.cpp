#include <errors.hpp>

FileNotFoundError::FileNotFoundError(std::string filename) : 
    std::runtime_error{"File not found: " + filename}
{}

InvalidFormatError::InvalidFormatError() :
    std::runtime_error{"Input had invalid format"}
{}

InvalidFormatError::InvalidFormatError(std::string s) :
    std::runtime_error{s}
{}
