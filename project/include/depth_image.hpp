#ifndef DEPTH_IMAGE_HPP
#define DEPTH_IMAGE_HPP
#include <pgm_image.hpp>
#include <histogram.hpp>
#include <stdexcept>
#include <vector>

class DepthImage {
    public:
    DepthImage(const PgmImage& img);
    const int height;
    const int width;
    const std::vector<int> pixelData;
    const Histogram histogram;

    std::tuple<double, double> compareTo(const DepthImage& o);

    class InvalidComparisonError: public std::runtime_error {
        public:
          InvalidComparisonError(int i, int q) :
            std::runtime_error(std::string("Expected length of ") +\
                    std::to_string(i) + \
                    std::string(", got length of ") +\
                    std::to_string(q)) {
            }
    };
};
#endif
