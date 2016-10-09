#ifndef HISTOGRAM_HPP
#define HISTOGRAM_HPP
#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include <numeric>
#include <cmath>
#include <vector>
#include <errors.hpp>

/**
 * Read a list of integers between 0 and 255, and does some basic stats on them.
 */
class Histogram {

public:
    static constexpr size_t binCount = 64;
    using BinType = std::array<int, binCount>;
    using NormalizedBinType = std::array<double, binCount>;

    Histogram(const std::vector<int> &list);
    Histogram(const Histogram& h1, const Histogram &h2);
    Histogram();

    void printStemAndLeaf() const;

    inline const BinType& getBins() const { return bins; }

    inline const NormalizedBinType& getNormalizedBins() const {
        return normalizedBins;
    }

    double dotProduct(const Histogram &other) const;

    double minimumSum(const Histogram &other) const;
               

    private:
    BinType bins;
    NormalizedBinType normalizedBins;
};

#endif
