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
    size_t size;
    
    FileBuff() {
        begin = nullptr;
        end = nullptr;
        fd = -1;
    }
    
    inline void readFile(const std::string& fname) {
        unmap();
        fd = open(fname.c_str(), O_RDONLY);
        if(fd <= 0) {
            throw std::runtime_error("Could not open!");
        }
        struct stat s;
        fstat(fd, &s);
        size = s.st_size;
        auto ptr = mmap(nullptr,
                      size,
                      PROT_READ,
                      MAP_PRIVATE,
                      fd,
                      0);
        if(reinterpret_cast<long>(ptr) == -1) {
            throw std::runtime_error("Could not read!");
        }
        begin = static_cast<char *>(ptr);
        end = begin + size;
        madvise(ptr, size, MADV_SEQUENTIAL);
    }
    
    ~FileBuff() {
        unmap();
    }
    
    inline void unmap() {
        if(fd > 0) {
            close(fd);
        }
        if(begin != nullptr) {
            munmap(static_cast<void *>(begin), size);
        }
    }
};


DepthImage::DepthImage(const std::string& filename) :
    fileName(filename)
{
    FileBuff fb;
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
