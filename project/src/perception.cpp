#include <perception.hpp>

Perception::Perception() :
  bins{}, bias(0)
{}

double Perception::getValue(const Histogram &h) {
    auto &b = h.getNormalizedBins();
    double sum = 0;
    for(int i = 0; i < Histogram::binCount; i++) {
        sum += (bins[i] * b[i]);
    }
    return sum + bias;
}

void Perception::train(const Histogram &h, int d) {
    auto &b = h.getNormalizedBins();
    double y = getValue(h);
    for(int i = 0; i < binCount; ++i) {
        bins[i] = bins[i] + ((d - y) * b[i]);
    }
    bias = bias + (d - y);
}

std::ostream& operator<<(std::ostream& os, const Perception& p) {
    auto& b = p.getBins();
    for(int i = 0; i < Perception::binCount; ++i) {
        os << b[i] << " ";
    }
    return os << p.getBias();
}
