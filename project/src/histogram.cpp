#include <histogram.hpp>

inline void numberCheck(int q) {
    if(q < 0 || q > 255) {
        throw BadNumberError(q);
    }
}

Histogram::Histogram() :
    bins{},
    normalizedBins{}
{}

Histogram::Histogram(const Histogram& h1, const Histogram &h2) :
    bins{},
    normalizedBins{}
{
    auto bins1 = h1.getBins();
    auto bins2 = h2.getBins();
    auto numNumbers = 0;
    for(unsigned int i = 0; i < binCount; ++i) {
        bins[i] = bins1[i] + bins2[i];
        numNumbers += (bins1[i] + bins2[i]);
    }
    for(unsigned int i = 0; i < binCount; ++i) {
        normalizedBins[i] = bins[i] / (double) numNumbers;
    }
}

Histogram::Histogram(const std::vector<int> &numbers) : 
    bins{},
    normalizedBins{}
{
    for(auto q: numbers) {
        numberCheck(q);
        size_t index = std::floor(q / 4.0);
        bins[index] = bins[index] + 1;
    }
    const auto numNumbers = numbers.size();
    for(size_t i = 0; i < binCount; ++i) {
        normalizedBins[i] = (double) bins[i] / numNumbers;
    }
}

void Histogram::printStemAndLeaf() const {
    for(auto q: bins) {
        std::cout << q << std::endl;
    }
}

double Histogram::dotProduct(const Histogram &other) const {
    auto n = other.getNormalizedBins();
    return std::inner_product(normalizedBins.begin(),
            normalizedBins.end(),
            n.begin(), 
            (double) 0.0);
}

double Histogram::minimumSum(const Histogram &other) const {
    double accum = 0;
    auto otherBins = other.getNormalizedBins();
    for(size_t i = 0; i < binCount; i++) {
        auto a = std::min(normalizedBins[i], otherBins[i]);
        accum += a;
    }
    return accum;
}
