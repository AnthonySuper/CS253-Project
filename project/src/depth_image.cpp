#include <depth_image.hpp>

DepthImage::DepthImage(const PgmImage &i) :
    height(i.getHeight()),
    width(i.getWidth()),
    pixelData(i.getPixels()),
    histogram(pixelData) 
{
}

DepthImage::DepthImage(DepthImage&& o) :
    height(o.height),
    width(o.width),
    pixelData(std::move(o.pixelData)),
    histogram(o.histogram)
{
    o.height = 0;
    o.width = 0;
    o.histogram = Histogram{};
}

DepthImage::DepthImage(PgmImage &&i) :
    height(i.height),
    width(i.width),
    pixelData(std::move(i.pixelData)),
    histogram(pixelData)
{
    i.height = 0;
    i.width = 0;
    i.maxValue = 0;
}

double DepthImage::minimumSumComparison(const DepthImage& o) {
    return histogram.minimumSum(o.histogram);
}
