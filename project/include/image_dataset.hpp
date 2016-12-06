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
    using ImagePtr = DepthImage*;
    using iterator = DepthImage*;
    using const_iterator = DepthImage const *;

    static std::unique_ptr<ImageDataset> fromFile(FileBuff &fb);
    
    ImageDataset(size_t size) {
        imageBuffer = (DepthImage*) std::malloc(sizeof(*imageBuffer) * size);
        _size = size;
    }
    
    ImageDataset(const ImageDataset&) = delete;
    ImageDataset(ImageDataset&) = delete;
    
    ImageDataset& operator=(ImageDataset &o) = delete;
    
    
    ImageDataset(ImageDataset&& id);
    
    /*
    ImageDataset& operator=(ImageDataset&& id) :
        _size(id._size),
        imageBuffer(id.imageBuffer),
        classList(std::move(id.classList))
    {
        id._size = 0;
        id.imageBuffer = nullptr;
    }
     */
    
    ~ImageDataset();

    size_t size() const {
        return _size;
    }
    
    inline const DepthImage& refAt(int i) const {
        return *(imageBuffer + i);
    }
    
    inline DepthImage* ptrAt(int i) {
        return (imageBuffer + i);
    }

    inline iterator begin() {
        return imageBuffer;
    }

    inline iterator end() {
        return imageBuffer + _size + 1;
    }

    inline const_iterator begin() const {
        return imageBuffer;
    }

    inline const_iterator end() const {
        return imageBuffer + _size + 1;
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
    size_t _size;
    DepthImage *imageBuffer;
    std::set<int> classList;
};

#endif
