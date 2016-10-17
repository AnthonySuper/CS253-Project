#ifndef _IMAGE_GROUP_HPP_
#define _IMAGE_GROUP_HPP_
#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <image_dataset.hpp>
#include <ostream>
#include <typeinfo>

/**
 * Represents a group of images, which are most likely similar to each other.
 */
class ImageGroup {
public:
    using Dataset = std::shared_ptr<ImageDataset>;

    friend std::ostream& operator<<(std::ostream&, const ImageGroup&);
    virtual ~ImageGroup();
    
    /**
     * Merge another group into this group.
     * Does not modify the other group.
     */
    virtual void merge(ImageGroup&) = 0;

    virtual operator std::string() const;

    /**
     * Determine the similarity of this group to another group.
     * If the two groups are of the same type, this method will likely be
     * much faster than otherwise, but all implementers should ensure that
     * they work for arbitrary group types.
     */
    virtual double similarityTo(ImageGroup&) = 0;

    /**
     * Get the indexes of images that belong to this group.
     */
    inline std::vector<unsigned int> getIndexes() const {
        return indexes;
    }

    /**
     * Get the dataset that belongs to this group.
     */
    inline std::shared_ptr<ImageDataset> getDataset() const {
        return dataset;
    }

    /**
     * Abstract factory that creates a group from a Dataset and an initial
     * index. You can then modify this group using the ImageGroup::merge method
     */
    class Factory {
    public:
        /**
         * Create a group given a single image, represented as a dataset
         * and an index into it.
         */
        virtual ImageGroup* create(Dataset ds, unsigned int index) = 0;
    };

protected:
    std::vector<unsigned int> indexes;

    std::shared_ptr<ImageDataset> dataset;

    ImageGroup() {} 
};


std::ostream& operator<<(std::ostream&, const ImageGroup&);
#endif
