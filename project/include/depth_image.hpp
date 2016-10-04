#ifndef DEPTH_IMAGE_HPP
#define DEPTH_IMAGE_HPP
#include <pgm_image.hpp>
#include <histogram.hpp>
#include <stdexcept>
#include <vector>

class DepthImage {
    public:
    DepthImage(const PgmImage& img);
    DepthImage(const PgmImage&& img);
    const int height;
    const int width;
    const std::vector<int> pixelData;
    const Histogram histogram;
    double minimumSumComparison(const DepthImage&);
};
#endif
