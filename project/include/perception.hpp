#ifndef _PERCEPTION_HPP
#define _PERCEPTION_HPP
#include <ostream>
#include <histogram.hpp>
#include <array>

class Perception {
public:
    static constexpr int binCount = 64;
    using BinType = std::array<double, binCount>;
    double getValue(const Histogram& h) const;
    
    void train(const Histogram &h, int d);
    
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
