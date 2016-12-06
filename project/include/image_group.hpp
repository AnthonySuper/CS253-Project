#ifndef _IMAGE_GROUP_HPP_
#define _IMAGE_GROUP_HPP_
#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <perception_trainer.hpp>
#include <image_dataset.hpp>
#include <ostream>
#include <typeinfo>
#include <iostream>
#include <unordered_map>
#include <algorithm>

/**
 * Represents a group of images, which are most likely similar to each other.
 */
class ImageGroup {
public:
    using ImagePtr = DepthImage*;

    friend std::ostream& operator<<(std::ostream&, const ImageGroup&);

    ImageGroup(ImagePtr ip);

    /**
     * Merge another group into this group.
     * Does not modify the other group.
     */
    void merge(ImageGroup&);

    virtual operator std::string() const;

    /**
     * Determine the similarity of this group to another group.
     */

    inline double similarityTo(ImageGroup& o, const PerceptionTrainer& pt)
    {
        double sum = 0;
        for(const Perception& p: pt.getPerceptions()) {
            double vala = p.getValue(hg);
            double valb = p.getValue(o.hg);
            double under = (vala - valb) + 0.001;
            under = under*under;
            sum += 1 / under;
        }
        return sum;
    }

    /**
     * Get the images that belong to this group.
     */
    inline std::vector<ImagePtr> getImages() const {
        return images;
    }

    inline int size() const {
        return images.size();
    }

    /**
     * Return the number of times the most occuring class occurs.
     */
    int getMostOccuringClassCount() const;

protected:
    std::vector<ImagePtr> images;

    Histogram hg;

    ImageGroup() {} 
};


std::ostream& operator<<(std::ostream&, const ImageGroup&);
#endif
