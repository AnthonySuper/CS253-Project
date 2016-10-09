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
    using ptr_type = std::shared_ptr<ImageDataset>;

    static ptr_type fromFile(std::string fname);
    void emplace_back(std::string fname);
    const DepthImage& at(int index);
    size_t size() {
        return images.size();
    }

    std::vector<DepthImage>::const_iterator begin() const {
        return images.begin();
    }

    std::vector<DepthImage>::const_iterator end() const {
        return images.end();
    }
protected:
    ImageDataset() = default;
    std::vector<DepthImage> images;
    friend ptr_type ImageDataset::fromFile(std::string);
};

#endif
