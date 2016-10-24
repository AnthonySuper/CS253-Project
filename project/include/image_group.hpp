#ifndef _IMAGE_GROUP_HPP_
#define _IMAGE_GROUP_HPP_
#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <image_dataset.hpp>
#include <ostream>
#include <typeinfo>
#include <unordered_map>

/**
 * Represents a group of images, which are most likely similar to each other.
 */
class ImageGroup {
public:
    using ImagePtr = std::shared_ptr<DepthImage>;

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
     * Get the images that belong to this group.
     */
    inline std::vector<ImagePtr> getImages() const {
        return images;
    }

    /**
     * Return a fitness measure, in terms of the most occuring images,
     * and the total number of images in the group.
     * To get a percentage value from this, simply divide the first element
     * of the tuple by the second element
     *
     * \return <mostOccuring, totalImages>
     */
    std::tuple<int, int> getFitness() const;

    /**
     * Abstract factory that creates a group from a Dataset and an initial
     * index. You can then modify this group using the ImageGroup::merge method
     */
    class Factory {
    public:
        /**
         * Create a group given a single image.
         */
        virtual ImageGroup* create(ImagePtr) = 0;
        virtual ~Factory();
    };

protected:
    std::vector<ImagePtr> images;

    void appendImages(std::vector<ImagePtr> images);

    ImageGroup() {} 
};


std::ostream& operator<<(std::ostream&, const ImageGroup&);
#endif
