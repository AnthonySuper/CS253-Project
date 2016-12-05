#ifndef _IMAGE_DATASET_HPP
#define _IMAGE_DATASET_HPP
#include <depth_image.hpp>
#include <vector>
#include <fstream>
#include <memory>
#include <set>
#include <future>

/**
 * A class which holds a set of images for further analysis.
 * It can only be accessed from the static member fromFile, which returns
 * a shared_ptr<ImageDataset>.
 * The reason for this is that copying these datasets is *hugely* expensive.
 */
class ImageDataset {
public:
    using ImagePtr = std::shared_ptr<DepthImage>;

    static ImageDataset fromFile(FileBuff &fb);

    void emplace_back(const DepthImage& img);
    
    void emplace_back(ImagePtr);

    ImagePtr at(int index);

    size_t size() const {
        return images.size();
    }
    
    void reserve(size_t size) {
        images.reserve(size);
    }
    
    
    inline const DepthImage& refAt(int i) const {
        return *images.at(i);
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

    inline bool hasClass(int i) const {
        return classList.find(i) != classList.end();
    }
    
    inline const std::set<int>& getClassList() const {
        return classList;
    }

    ImageDataset() = default;

protected:
    void addClass(int);
    std::vector<ImagePtr> images;
    std::set<int> classList;
};

#endif
