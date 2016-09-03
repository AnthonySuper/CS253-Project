#ifndef _NUMBER_LIST_HPP
#define _NUMBER_LIST_HPP
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <cmath>

/**
 * Read a list of integers between 0 and 255, and does some basic stats on them.
 */
class NumberList {
public:
    NumberList(std::string filename);
    void printStemAndLeaf();

    class FileNotFoundError : public std::runtime_error {
    public:
        FileNotFoundError(std::string f) : 
            runtime_error("File " + f + " not found"){}
    };

    class BadNumberError : public std::runtime_error {
    public:
        BadNumberError(int i) :
            runtime_error("Recieved unexpected number " + std::to_string(i))
        {}
    };

    class BadInputError : public std::runtime_error {
    public:
        BadInputError() :
            runtime_error("Bad input recieved") {}
    };

    private:
    std::vector<int> numbers;
    int counts[64] = {0};

};

#endif
