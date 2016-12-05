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
#include <cstdint>
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
    Histogram(const std::vector<uint8_t> &list);
    /**
     * Construct a histogram by merging two histograms.
     * This adds their binned values together bin-wise, then calculates the
     * new averages for each bin.
     */
    inline Histogram(const Histogram& h1, const Histogram &h2) :
    bins{},
    normalizedBins{},
    numNumbers(h1.numNumbers + h2.numNumbers)
    {
        auto bins1 = h1.getBins();
        auto bins2 = h2.getBins();
        for(unsigned int i = 0; i < binCount; ++i) {
            bins[i] = bins1[i] + bins2[i];
        }
        for(unsigned int i = 0; i < binCount; ++i) {
            normalizedBins[i] = bins[i] / (double) numNumbers;
        }
    }
    
    /**
     * Default constructor merely constructs a bunch of zero-valued bins.
     */
    Histogram();
    
    Histogram(Histogram &&o) = default;
    Histogram& operator=(const Histogram &o) = default;
    Histogram& operator=(Histogram &&o) = default;
    
    Histogram(const Histogram &o) :
    bins(o.bins), normalizedBins(o.normalizedBins), numNumbers(o.numNumbers) {}

    /**
     * Print a stem-and-leaf chart to standard output. 
     * Highly likely to be removed in the future.
     */
    void printStemAndLeaf() const;

    /**
     * Obtain a reference to the bins in this histogram
     */
    inline const BinType& getBins() const { return bins; }

    inline void inc(int in) {
        ++numNumbers;
        bins[numToIndex(in)]++;
    }

    inline void finalize() {
        double n = (double) numNumbers;
        for(unsigned int i = 0; i < binCount; ++i) {
            normalizedBins[i] = bins[i] / n;
        }
    }
    
    inline void merge(const Histogram &o) {
        numNumbers += o.numNumbers;
        for(unsigned int i = 0; i < binCount; ++i) {
            bins[i] += o.bins[i];
        }
        for(unsigned int i = 0; i < binCount; ++i) {
            normalizedBins[i] = (double) bins[i] / numNumbers;
        }
    }

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

    /**
     * See if two histograms are exactly the same
     */
    bool operator==(const Histogram&o) const;
    
    static int numToIndex(int index) {
        return lookup[index];
    }
    
    static const int lookup[];
    
protected:
    BinType bins;
    NormalizedBinType normalizedBins;
    int numNumbers = 0;
};

#endif
