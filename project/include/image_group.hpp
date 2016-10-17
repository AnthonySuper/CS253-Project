#ifndef _IMAGE_GROUP_HPP_
#define _IMAGE_GROUP_HPP_
#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <image_dataset.hpp>
#include <ostream>
#include <typeinfo>


class ImageGroup {
public:
    using Dataset = std::shared_ptr<ImageDataset>;

    friend std::ostream& operator<<(std::ostream&, const ImageGroup&);
    virtual ~ImageGroup();

    virtual void merge(ImageGroup&) = 0;

    virtual operator std::string() const;

    virtual double similarityTo(ImageGroup&) = 0;


    inline std::vector<unsigned int> getIndexes() const {
        return indexes;
    }

    inline std::shared_ptr<ImageDataset> getDataset() const {
        return dataset;
    }

    class Factory {
    public:
        virtual ImageGroup* create(Dataset ds, unsigned int index) = 0;
    };

protected:
    std::vector<unsigned int> indexes;

    std::shared_ptr<ImageDataset> dataset;

    ImageGroup() {} 
};


std::ostream& operator<<(std::ostream&, const ImageGroup&);
#endif
