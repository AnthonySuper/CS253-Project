#include <histogram.hpp>

inline void numberCheck(int q) {
    if(q < 0 || q > 255) {
        throw BadNumberError(q);
    }
}

Histogram::Histogram() :
    bins{},
    normalizedBins{},
    numNumbers(0)
{}


Histogram::Histogram(const std::vector<uint8_t> &numbers) :
    bins{},
    normalizedBins{},
    numNumbers(numbers.size())
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

bool Histogram::operator==(const Histogram&o) const {
    return (bins == o.bins && normalizedBins == o.normalizedBins);
}
