#include <depth_image.hpp>

DepthImage::DepthImage(const PgmImage &i) :
    height(i.getHeight()),
    width(i.getWidth()),
    pixelData(i.getPixels()),
    histogram(pixelData) 
{
}

DepthImage::DepthImage(const PgmImage &&i) :
    height(i.getHeight()),
    width(i.getWidth()),
    pixelData(std::move(i.pixelData)),
    histogram(pixelData)
{
}

double DepthImage::minimumSumComparison(const DepthImage& o) {
    return histogram.minimumSum(o.histogram);
}
