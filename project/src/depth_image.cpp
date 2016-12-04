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
    auto currentPos = f.tellg();
    if(currentPos == -1) {
        throw std::runtime_error("Not supported!");
    }
    auto rdbuff = f.rdbuf();
    auto size = rdbuff->pubseekoff(0, f.end, f.in);
    rdbuff->pubseekoff(currentPos, f.beg);
    size_t buffSize = size - currentPos;
    char *buff = new char[buffSize];
    rdbuff->sgetn(buff, buffSize);
    char *scan = buff;
    int tmp = -1;
    for(auto scan = buff; scan != buff + buffSize; scan++) {
        if(*scan == '\n' || *scan == '\t' || *scan == ' ') {
            if(tmp > -1) {
                if(tmp <= 255) {
                    histogram.inc(tmp);
                    tmp = -1;
                }
                else {
                    std::cout << "Got a tmp of " << tmp << std::endl;
                    throw std::runtime_error("Bad format: " + std::to_string(tmp));
                    delete[] buff;
                }
            }
        }
        else if(*scan >= '0' && *scan <= '9') {
            if(tmp == -1) {
                tmp = 0;
            }
            tmp = tmp * 10 + static_cast<int>((*scan - '0'));
        }
        else {
            delete[] buff;
            throw std::runtime_error("Failure will robinson!");
        }
    }
    if(tmp > -1) {
        histogram.inc(tmp);
    }
    delete[] scan;
    histogram.finalize();
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
