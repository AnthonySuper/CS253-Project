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
    size_t buffSize;
    size_t fileSize;
    int fd = -1;
    
    
    FileBuff() {
        begin = nullptr;
        end = nullptr;
        buffSize = 0;
    }
    
    inline void readFile(const std::string &f) {
        readFile(f.c_str());
    }
    
    __attribute__((always_inline)) inline void readFile(const char *ptr) {
        closeIfNeeded();
        /*
        FILE *fp = fopen(ptr, "r");
        struct stat s;
        stat(ptr, &s);
        fileSize = s.st_size;
        if(fileSize > buffSize) {
            realloc();
        }
        ssize_t rd = 0;
        while(rd < fileSize) {
            rd += fread(begin, 1, fileSize - rd, fp);
        }
         fclose(fp);
        */
        fd = open(ptr, O_RDONLY);
        if(fd <= 0) {
            throw std::runtime_error("Could not open!");
        }
        struct stat s;
        fstat(fd, &s);
        fileSize = s.st_size;
        begin = (char *) mmap(nullptr,
                              fileSize,
                              PROT_READ | PROT_WRITE,
                              MAP_PRIVATE,
                              fd, 0);
        madvise((void *) begin, fileSize, MADV_SEQUENTIAL);
        /*
        if(fileSize > buffSize) {
            realloc();
        }
        ssize_t rd = 0;
        while(rd < fileSize) {
            rd += read(fd, begin + rd, fileSize - rd);
        }
         */
        end = begin + fileSize;
        // close(fd);
    }
    
    inline void closeIfNeeded() {
        if(fd > 0 && begin != nullptr) {
            munmap((void *) begin, fileSize);
            close(fd);
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
        closeIfNeeded();
        /*
        if(begin != nullptr) {
            free(begin);
        }
         */
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
     * Obtain a reference to the histogram made from this DepthImage
     */
    inline const Histogram& getHistogram() const { return histogram; }

    inline const char * getFileName() const { return fileName; }
    /**
     * Obtain the numerical category of the image.
     */
    inline int getCategory() const {
        if(category > 0) {
            return category;
        }
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
    Histogram histogram;
    mutable int category = -2;
};

inline std::ostream& operator<<(std::ostream& os, const DepthImage& i) {
    os << (std::string) i;
    return os;
}

#endif
