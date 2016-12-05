#ifndef DEPTH_IMAGE_HPP
#define DEPTH_IMAGE_HPP
#include <histogram.hpp>
#include <stdexcept>
#include <string>
#include <vector>
#include <istream>
#include <fstream>
#include <sstream>
#include <regex>
#include <cstdint>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <thread>

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
    
    inline void readFile(const std::string &f) {
        readFile(f.c_str());
    }
    
    inline void readFile(const char *ptr) {
        if(fd != 0) {
            close(fd);
        }
        fd = open(ptr, O_RDONLY);
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
        // give us extra space
        buffSize = (sizeof(char) * fileSize) + 1028*20;
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



/**
 * A DepthImage represents, as the name implies, a depth-based image.
 * It contains the pixel data, height, width, and other information for these
 * images.
 * It also contains a histogram of the image's pixel values, for analysis.
 *
 * Note that depth images are actually rather large.
 * The histograms are quite big, as are the pixel values.
 * We provide utilities like the ImageDataset to allow you to avoid copying.
 * This also provides a move constructor.
 */
class DepthImage {
public:
    /**
     * Obtain a DepthImage by reading from a file.
     * \param filename the file to read from.
     * \throws FileNotFoundError in case the file is invalid
     * \throws InvalidFormatError when the file is not a valid PGM image
     */
    DepthImage(const char *fname, size_t nameSize, FileBuff &);
    /**
     * Standard move constructor.
     */
    DepthImage(DepthImage &&o);
    /**
     * Standard copy constructor.
     */
    DepthImage(const DepthImage &o);
    /**
     * Compare two depth images by their minimum sum, which is 
     * equivalent to the intersection of their histograms.
     */
    double minimumSumComparison(const DepthImage&) const;
    /**
     * Get the height of this DepthImage, in number of pixels.
     * \return the height
     */
    inline int getHeight() const { return height; }
    /**
     * Get the width of this DepthImage, in number of pixels.
     * \return the width
     */
    inline int getWidth() const { return width; }
    /**
     * Obtain the pixel data of this DepthImage.
     * The vector should be considered in the context of the width and height
     * of this DepthImage.
     */
    inline const std::vector<uint8_t>& getPixelData() const { return pixelData; }
    /**
     * Obtain a reference to the histogram made from this DepthImage
     */
    inline const Histogram& getHistogram() const { return histogram; }

    inline const char * getFileName() const { return fileName; }
    /**
     * Obtain the numerical category of the image.
     */
    inline int getCategory() const {
        return category;
    }
    /**
     * Convert this DepthImage to a string, which provides some debugging
     * information.
     */
    explicit operator std::string() const;
protected:
    const char *fileName;
    size_t nameSize;
    int height;
    int width;
    std::vector<uint8_t> pixelData;
    Histogram histogram;
    int category = -2;
};

inline std::ostream& operator<<(std::ostream& os, const DepthImage& i) {
    os << (std::string) i;
    return os;
}

#endif
