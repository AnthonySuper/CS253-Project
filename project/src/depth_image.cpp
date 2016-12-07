#include <depth_image.hpp>

#define IS_SPACE(c)(c < '0' || c > '9')

DepthImage::DepthImage(const char *fname, size_t fsize, FileBuff& fb) :
    fileName(fname), nameSize(fsize)
{
    
    fb.readFile(fname);
    auto scan = fb.begin + 2;
    int numRead = 0;
    int tmp = -1;
    int loopCount = fb.end - scan;
    // for(auto scan = b; scan != fb.end; ++scan) {
    for(int i = 0; i < loopCount; ++i) {
        char c = *scan;
        if(IS_SPACE(c)) {
            if(tmp > -1) {
                if(numRead > 3) {
                    histogram.inc(tmp);
                }
                else {
                    ++numRead;
                }
                tmp = -1;
            }
        }
        // They promised no error cases
        else {
            if(tmp < 0) {
                tmp = 0;
            }
            // tmp = tmp * 10 + ((*scan - '0'));
            tmp = ((unsigned) tmp * 10) + c - '0';
        }
        ++scan;
    }
    if(tmp > -1 && tmp < 255) {
        histogram.inc(tmp);
    }
    histogram.finalize();
}

DepthImage::DepthImage(DepthImage&& o) :
    fileName(std::move(o.fileName)),
    nameSize(o.nameSize),
    histogram(std::move(o.histogram)),
    category(o.category)
{
}

DepthImage::DepthImage(const DepthImage &o) :
    fileName(o.fileName),
    nameSize(o.nameSize),
    histogram(o.histogram),
    category(o.category)
{}

DepthImage::operator std::string() const  {
    std::stringstream s;
    s << ", fileName: \"" << fileName << "\"}";
    return s.str();
}

