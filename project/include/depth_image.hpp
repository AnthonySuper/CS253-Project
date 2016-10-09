#ifndef DEPTH_IMAGE_HPP
#define DEPTH_IMAGE_HPP
#include <histogram.hpp>
#include <stdexcept>
#include <string>
#include <vector>
#include <istream>
#include <fstream>
#include <sstream>

class DepthImage {
public:
    DepthImage(std::string filename);
    DepthImage(DepthImage &&o);
    DepthImage(const DepthImage &o);
    double minimumSumComparison(const DepthImage&) const;
    inline int getHeight() const { return height; }
    inline int getWidth() const { return width; }
    inline const std::vector<int>& getPixelData() const { return pixelData; }
    inline const Histogram& getHistogram() const { return histogram; }
    inline const std::string& getFileName() const { return fileName; }
    explicit operator std::string() const;
protected:
    std::string fileName;
    int height;
    int width;
    std::vector<int> pixelData;
    Histogram histogram;
};

inline std::ostream& operator<<(std::ostream& os, const DepthImage& i) {
    os << (std::string) i;
    return os;
}

#endif
