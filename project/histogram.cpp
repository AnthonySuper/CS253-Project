#include <histogram.hpp>

Histogram Histogram::fromFile(std::string fname) {
    std::ifstream st(fname);
    std::vector<int> numbers;
    if(! st.is_open()) {
        throw FileNotFoundError(fname);
    }
    st >> std::skipws;
    int x;
    while(st >> x) {
        if(st.fail() && ! st.eof()) {
            throw BadInputError();
        }
        numbers.push_back(x);
    }
    return Histogram(numbers);
}

Histogram::Histogram(std::vector<int> i) : numbers(i),
  bins(binCount, 0), 
  normalizedBins(binCount, 0) {
    for(auto q: numbers) {
        if(q > 255 || q < 0) {
            throw BadNumberError(q);
        }
        size_t index = std::floor(q / 4.0);
        bins[index] = bins[index] + 1;
    }
    const auto numNumbers = numbers.size();
    for(int i = 0; i < binCount; ++i) {
        normalizedBins[i] = (double) bins[i] / numNumbers;
    } 
}

void Histogram::printStemAndLeaf() const {
    for(auto q: bins) {
        std::cout << q << std::endl;
    }
}

double Histogram::zipNormalized(const Histogram &other) const {
    auto n = other.getNormalizedBins();
    return std::inner_product(normalizedBins.begin(),
            normalizedBins.end(),
            n.begin(), (double) 0.0);
}
