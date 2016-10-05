#ifndef HISTOGRAM_HPP
#define HISTOGRAM_HPP
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <numeric>
#include <iterator>
#include <cmath>
#include <errors.hpp>

/**
 * Read a list of integers between 0 and 255, and does some basic stats on them.
 */
class Histogram {
public:
    Histogram(const std::vector<int> &list);
    Histogram();

    void printStemAndLeaf() const;

    inline std::vector<int> getBins() const { return bins; }

    inline std::vector<double> getNormalizedBins() const {
        return normalizedBins;
    }

    double dotProduct(const Histogram &other) const;

    double minimumSum(const Histogram &other) const;
               

    private:
    std::vector<int> bins;
    std::vector<double> normalizedBins;
    static const size_t binCount = 64;
};

#endif
