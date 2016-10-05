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
    double minimumSumComparison(const DepthImage&) const;
    inline int getHeight() const { return height; }
    inline int getWidth() const { return width; }
    inline const std::vector<int>& getPixelData() const { return pixelData; }
    inline const Histogram& getHistogram() const { return histogram; }
    protected:
    int height;
    int width;
    std::vector<int> pixelData;
    Histogram histogram;
};
#endif
