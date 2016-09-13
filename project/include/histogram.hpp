#ifndef HISTOGRAM_HPP
#define HISTOGRAM_HPP
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <numeric>
#include <stdexcept>
#include <cmath>

/**
 * Read a list of integers between 0 and 255, and does some basic stats on them.
 */
class Histogram {
public:
    static Histogram fromFile(std::string filename);

    Histogram(std::vector<int> list);

    void printStemAndLeaf() const;

    inline std::vector<int> getBins() const { return bins; }

    inline std::vector<double> getNormalizedBins() const {
        return normalizedBins;
    }

    double zipNormalized(const Histogram &other) const;

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
    std::vector<int> bins;
    std::vector<double> normalizedBins;
    static const size_t binCount = 64;
};

#endif
