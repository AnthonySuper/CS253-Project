#ifndef DEPTH_IMAGE_HPP
#define DEPTH_IMAGE_HPP
#include <pgm_image.hpp>
#include <histogram.hpp>
#include <stdexcept>
#include <vector>

class DepthImage {
    public:
    DepthImage(const PgmImage& img);
    DepthImage(PgmImage&& img);
    DepthImage(DepthImage &&o);
    DepthImage(DepthImage &o) = default;
    double minimumSumComparison(const DepthImage&);
    inline int getHeight() { return height; }
    inline int getWidth() { return width; }
    inline const std::vector<int>& getPixelData() { return pixelData; }
    inline const Histogram& getHistogram() { return histogram; }
    protected:
    int height;
    int width;
    std::vector<int> pixelData;
    Histogram histogram;
};
#endif
