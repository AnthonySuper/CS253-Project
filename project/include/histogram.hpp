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
 * A histogram which uses binned values of length 4, to a max value of 255.
 * It stores the individual bins in two formats: their counts, and normalized.
 */
class Histogram {

public:
    static constexpr size_t binCount = 64;
    /**
     * The type of the array of binned values we use.
     */
    using BinType = std::array<int, binCount>;
    /**
     * The type of the array of NormalizedBinValues we use.
     */
    using NormalizedBinType = std::array<double, binCount>;
    /**
     * Construct a Histogram from a list of numbers.
     * All numbers must be between 0 and 255.
     * \param list the list of numbers to construct a histogram from.
     */
    Histogram(const std::vector<int> &list);
    /**
     * Construct a histogram by merging two histograms.
     * This adds their binned values together bin-wise, then calculates the
     * new averages for each bin.
     */
    Histogram(const Histogram& h1, const Histogram &h2);
    /**
     * Default constructor merely constructs a bunch of zero-valued bins.
     */
    Histogram();

    /**
     * Print a stem-and-leaf chart to standard output. 
     * Highly likely to be removed in the future.
     */
    void printStemAndLeaf() const;

    /**
     * Obtain a reference to the bins in this histogram
     */
    inline const BinType& getBins() const { return bins; }

    /**
     * Obtain a reference to the normalized bins in this histogram.
     */
    inline const NormalizedBinType& getNormalizedBins() const {
        return normalizedBins;
    }

    /**
     * Obtain the vector dot product of this histogram with another.
     */
    double dotProduct(const Histogram &other) const;

    /**
     * Obtain the minimum sum comparison of this histogram to another.
     * Note that this is equivalent to the intersection of the two histograms.
     */
    double minimumSum(const Histogram &other) const;
               
    private:
    BinType bins;
    NormalizedBinType normalizedBins;
};

#endif
