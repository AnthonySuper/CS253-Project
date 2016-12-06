#include <depth_image.hpp>

static void depthError(int in) {
    std::stringstream s;
    s << "Invalid depth, expected 255, got " << in;
    throw InvalidFormatError(s.str());
}


#define IS_SPACE(c)(c == ' ' || c == '\t' || c == '\n' || c == '\r')

DepthImage::DepthImage(const char *fname, size_t fsize, FileBuff& fb) :
    fileName(fname), nameSize(fsize)
{
    fb.readFile(fname);
    auto b = fb.begin + 2;
    int numRead = 0;
    int tmp = -1;
    for(auto scan = b; scan != fb.end; ++scan) {
        if(IS_SPACE(*scan)) {
            if(tmp != -1) {
                if(numRead > 3) {
                    histogram.inc(tmp);
                }
                tmp = -1;
                numRead++;
            }
        }
        else if((unsigned char) *scan - '0' <= '9' - '0') {
            if(tmp == -1) {
                tmp = 0;
            }
            tmp = tmp * 10 + ((*scan - '0'));
        }
        else {
            throw std::runtime_error("Failure will robinson!");
        }
    }
    if(tmp > -1 && tmp < 255) {
        histogram.inc(tmp);
    }
    histogram.finalize();
}

DepthImage::DepthImage(DepthImage&& o) :
    fileName(std::move(o.fileName)),
    height(o.height),
    width(o.width),
    pixelData(std::move(o.pixelData)),
    histogram(std::move(o.histogram))
{
    o.height = 0;
    o.width = 0;
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

