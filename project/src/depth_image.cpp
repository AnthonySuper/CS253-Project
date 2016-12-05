#include <depth_image.hpp>

static void depthError(int in) {
    std::stringstream s;
    s << "Invalid depth, expected 255, got " << in;
    throw InvalidFormatError(s.str());
}

static inline bool isSpace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}


DepthImage::DepthImage(const char *fname, size_t fsize, FileBuff& fb) :
    fileName(fname), nameSize(fsize)
{
    fb.readFile(fname);
    auto b = fb.begin;
    
    if(*(b++) != 'P' || *(b++) != '2') {
        throw InvalidFormatError("File lacked proper header");
    }
    auto skipWS = [&] {
        while(isSpace(*b))
            b++;
    };
    
    auto readVal = [&] {
        int tmp = 0;
        while(*b >= '0' && *b <= '9') {
            tmp = tmp * 10 + static_cast<int>((*b - '0'));
            ++b;
        }
        return tmp;
    };
    
    skipWS();
    int width = readVal();
    skipWS();
    int height = readVal();
    skipWS();
    int maxValue = readVal();
    skipWS();
    
    if(maxValue != 255) {
        depthError(maxValue);
    }
    if(height < 0 || width < 0) {
        throw InvalidFormatError("Height or width impossible");
    }
    int tmp = -1;
    for(auto scan = b; scan != fb.end; scan++) {
        if(isSpace(*scan)) {
            if(tmp > -1) {
                if(tmp <= 255) {
                    histogram.inc(tmp);
                    tmp = -1;
                }
                else {
                    std::cout << "Got a tmp of " << tmp << std::endl;
                    throw std::runtime_error("Bad format: " + std::to_string(tmp));
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
            throw std::runtime_error("Failure will robinson!");
        }
    }
    if(tmp > -1) {
        histogram.inc(tmp);
    }
    histogram.finalize();
    auto gc = [&](){
        if(category > -1) {
            return category;
        }
        const char *end = fileName + nameSize;
        while(*end != 's') {
            end--;
            if(end == fileName) {
                throw std::runtime_error("Invalid name, can't find class");
            }
        }
        end++;
        int tmp = 0;
        while(*end >= '0' && *end <= '9') {
            tmp = tmp * 10 + (*end -  '0');
            end++;
        }
        return tmp;
    };
    category = gc();
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

