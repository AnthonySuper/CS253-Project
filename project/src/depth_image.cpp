#include <depth_image.hpp>

static void depthError(int in) {
    std::stringstream s;
    s << "Invalid depth, expected 255, got " << in;
    throw InvalidFormatError(s.str());
}


DepthImage::DepthImage(std::string filename) :
    fileName(filename) 
{
    std::ifstream f(filename);
    f >> std::noskipws;
    int maxValue;
    // File must start with these two characters, so we disallow whitespace
    if(! f.is_open()) {
        throw FileNotFoundError(filename);
    }
    char c1 = '\0', c2 = '\0';
    f >> c1;
    f >> c2;
    if(c1 != 'P' || c2 != '2') {
        throw InvalidFormatError("File lacked proper header");
    }
    f >> std::skipws;
    // whitespace is now allowed again
    if(! (f >> width) || ! (f >> height) || ! (f >> maxValue)) {
        throw InvalidFormatError("Width, Height, or max value not given");
    }
    if(maxValue != 255) {
        depthError(maxValue);
    }
    if(height < 0 || width < 0) {
        throw InvalidFormatError("Height or width impossible");
    }
    int i;
    while(! f.eof() && f >> i) {
        if(f.fail()) {
            throw InvalidFormatError("[1] Read a non-number!");
        }
        if(i < 0 || i > 255) {
            throw BadNumberError(i);
        }
        pixelData.push_back(i);
    }
    if(! f.eof()) {
        throw InvalidFormatError("[2] Read a non-number!");
    }
    if(pixelData.size() != (unsigned int) height * width) {
        throw InvalidFormatError("File's list of ints is too big or small");
    }
    histogram = Histogram{pixelData};
}

DepthImage::DepthImage(DepthImage&& o) :
    fileName(std::move(o.fileName)),
    height(o.height),
    width(o.width),
    pixelData(std::move(o.pixelData)),
    histogram(o.histogram)
{
    o.height = 0;
    o.width = 0;
    o.histogram = Histogram{};
}

DepthImage::DepthImage(const DepthImage &o) :
    fileName(o.fileName),
    height(o.height),
    width(o.width),
    pixelData(o.pixelData),
    histogram(o.histogram)
{}

DepthImage::operator std::string() const  {
    std::stringstream s;
    s << "DepthImage{height: " << height << ", width: " << width;
    s << ", fileName: \"" << fileName << "\"}";
    return s.str();
}

std::vector<int> DepthImage::getSection(int x, int y, int h, int w) {
    if(x + w >= width || y + h >= height) {
        throw std::invalid_argument("out of bounds");
    }
    std::vector<int> fragment;
    fragment.reserve(h * w);
    for(int i = 0; i < h; ++i) {
        for(int j = 0; j < w; ++j) {
            int yOffset = width*(i + y);
            int xOffset = (j + x);
            fragment.emplace_back(pixelData.at(yOffset + xOffset));
        }
    }
    return fragment;
}

double DepthImage::minimumSumComparison(const DepthImage& o) const {
    return histogram.minimumSum(o.histogram);
}

int DepthImage::getCategory() {
    if(category > 0) {
        return category;
    }
    std::regex r("class(\\d+)_.*?\\.pgm");
    std::smatch sm;
    if(! std::regex_search(fileName, sm, r)) {
        throw std::runtime_error("DepthImage has no file name!");
    }
    if(sm.size() != 2) {
        throw std::runtime_error("Regex somehow matched badly" \
                " (this shouldn't happen)");
    }
    std::string s(sm[1]);
    category = stoi(s);
    return category;
}
