#ifndef _PERCEPTION_HPP
#define _PERCEPTION_HPP
#include <ostream>
#include <histogram.hpp>
#include <array>

class Perception {
public:
    static constexpr int binCount = 64;
    using BinType = std::array<double, binCount>;
    inline double getValue(const Histogram& h) const
    {
        auto &b = h.getNormalizedBins();
        double sum = 0;
        for(unsigned int i = 0; i < Histogram::binCount; ++i) {
            sum += (bins[i] * b[i]);
        }
        return sum + bias;
    }
    
    inline void train(const Histogram &h, int d) {
        auto &b = h.getNormalizedBins();
        double y = getValue(h);
        for(int i = 0; i < binCount; ++i) {
            bins[i] = bins[i] + ((d - y) * b[i]);
        }
        bias = bias + (static_cast<double>(d) - y);
    }
    
    Perception();
    

    const BinType& getBins() const {
        return bins;
    }

    const double getBias() const {
        return bias;
    }

protected:
    BinType bins;
    double bias;
};

std::ostream& operator<<(std::ostream& os, const Perception &p);

#endif
