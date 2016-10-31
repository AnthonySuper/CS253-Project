#ifndef _IMAGE_DATASET_HPP
#define _IMAGE_DATASET_HPP
#include <depth_image.hpp>
#include <vector>
#include <fstream>
#include <memory>

/**
 * A class which holds a set of images for further analysis.
 * It can only be accessed from the static member fromFile, which returns
 * a shared_ptr<ImageDataset>.
 * The reason for this is that copying these datasets is *hugely* expensive.
 * Most of the classes that analyze this use pointers internally, and store
 * indexes to images as opposed to an image itself.
 */
class ImageDataset {
public:
    using ImagePtr = std::shared_ptr<DepthImage>;

    static ImageDataset fromFile(std::string fname);

    void emplace_back(std::string fname);

    void emplace_back(const DepthImage& img);

    ImagePtr at(int index);

    size_t size() {
        return images.size();
    }

    const std::vector<ImagePtr>& getImages() const {
        return images;
    }

    inline std::vector<ImagePtr>::iterator begin() {
        return images.begin();
    }

    inline std::vector<ImagePtr>::iterator end() {
        return images.end();
    }

    inline std::vector<ImagePtr>::const_iterator begin() const {
        return images.begin();
    }

    inline std::vector<ImagePtr>::const_iterator end() const {
        return images.end();
    }

    ImageDataset() = default;

protected:
    std::vector<ImagePtr> images;
};

#endif
