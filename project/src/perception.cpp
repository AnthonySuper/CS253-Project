#include <perception.hpp>

Perception::Perception() :
  bins{}, bias(0)
{}



std::ostream& operator<<(std::ostream& os, const Perception& p) {
    auto& b = p.getBins();
    for(int i = 0; i < Perception::binCount; ++i) {
        os << b[i] << " ";
    }
    return os << p.getBias();
}
