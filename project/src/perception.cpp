#include <perception.hpp>

Perception::Perception() :
  bins{}, bias(0)
{}

void Perception::train(const Histogram &h, int d) {
    auto &b = h.getNormalizedBins();
    double y = getValue(h);
    for(int i = 0; i < binCount; ++i) {
        bins[i] = bins[i] + ((d - y) * b[i]);
    }
    bias = bias + (static_cast<double>(d) - y);
}

std::ostream& operator<<(std::ostream& os, const Perception& p) {
    auto& b = p.getBins();
    for(int i = 0; i < Perception::binCount; ++i) {
        os << b[i] << " ";
    }
    return os << p.getBias();
}
