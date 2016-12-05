#include <depth_image.hpp>

static void depthError(int in) {
    std::stringstream s;
    s << "Invalid depth, expected 255, got " << in;
    throw InvalidFormatError(s.str());
}


struct FileBuff {
    char *begin;
    char *end;
    int fd;
    size_t buffSize;
    size_t fileSize;
    
    FileBuff() {
        begin = nullptr;
        end = nullptr;
        fd = -1;
        buffSize = 0;
    }
    
    inline void readFile(const std::string& fname) {
        fd = open(fname.c_str(), O_RDONLY);
        if(fd <= 0) {
            throw std::runtime_error("Could not open!");
        }
        struct stat s;
        fstat(fd, &s);
        fileSize = s.st_size;
        if(fileSize < buffSize || buffSize == 0) {
            realloc();
        }
        end = begin + fileSize;
        ssize_t rd = 0;
        while(rd < fileSize) {
            rd += read(fd, begin + rd, fileSize - rd);
        }
    }
    
    inline void realloc() {
        if(begin != nullptr) {
            std::free(begin);
        }
        buffSize = sizeof(char) * fileSize * 1.2;
        begin = static_cast<char *>(std::malloc(buffSize));
        end = begin + fileSize;
    }
    
    ~FileBuff() {
        if(fd > 0) {
            close(fd);
        }
        if(begin != nullptr) {
            free(begin);
        }
    }
};


DepthImage::DepthImage(const std::string& filename) :
    fileName(filename)
{
    thread_local FileBuff fb;
    fb.readFile(filename);
    auto b = fb.begin;
    
    if(*(b++) != 'P' || *(b++) != '2') {
        throw InvalidFormatError("File lacked proper header");
    }
    auto skipWS = [&] {
        while(*b == '\n' || *b == '\t' || *b == ' ')
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
        if(*scan == '\n' || *scan == '\t' || *scan == ' ') {
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
