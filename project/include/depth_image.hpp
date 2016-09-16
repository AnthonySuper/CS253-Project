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
          InvalidComparisonError(int h, int w, int h2, int w2) :
            std::runtime_error(std::string("Expected image with dimensions ") +\
                    std::to_string(h) + ',' + std::to_string(w) +\
                    std::string(", got image with dimensions ") +\
                    std::to_string(h2) + ',' + std::to_string(w2)) {
            }
    };
};
#endif
