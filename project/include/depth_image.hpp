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
    DepthImage(std::string filename);
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
    inline const std::vector<int>& getPixelData() const { return pixelData; }

    /**
     * Get the pixels in a section of the image.
     */
    std::vector<int> getSection(int x, int y, int height, int width);
    /**
     * Obtain a reference to the histogram made from this DepthImage
     */
    inline const Histogram& getHistogram() const { return histogram; }
    /**
     * Obtain the filename this DepthImage was originally read from.
     * If the filename is "", this means that the DepthImage was created
     * via other means (currently not possible)
     */
    inline const std::string& getFileName() const { return fileName; }

    /**
     * Obtain the numerical category of the image.
     */
    int getCategory();
    /**
     * Convert this DepthImage to a string, which provides some debugging
     * information.
     */
    explicit operator std::string() const;
protected:
    std::string fileName;
    int height;
    int width;
    std::vector<int> pixelData;
    Histogram histogram;
    int category = -2;
};

inline std::ostream& operator<<(std::ostream& os, const DepthImage& i) {
    os << (std::string) i;
    return os;
}

#endif
