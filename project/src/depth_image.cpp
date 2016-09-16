#include <depth_image.hpp>

DepthImage::DepthImage(const PgmImage &i) :
    height(i.getHeight()),
    width(i.getWidth()),
    pixelData(i.getPixels()),
    histogram(pixelData) {
}

std::tuple<double, double> DepthImage::compareTo(const DepthImage& o) {
    if(height != o.height || width != o.width) {
        throw InvalidComparisonError(height, width, 
                o.height, o.width);
    }
    double s = histogram.minimumSum(o.histogram);
    double sum = 0;
    auto& otherData = o.pixelData;
    for(int i = 0; i < pixelData.size(); i++) {
        auto diff = (pixelData[i] - otherData[i]);
        auto square = diff * diff;
        sum += square;
    }
    return std::make_tuple(s, sum);
}
