#ifndef _HISTOGRAM_GROUP_HPP
#define _HISTOGRAM_GROUP_HPP
#include <image_group.hpp>
#include <histogram.hpp>

class HistogramGroup : public ImageGroup {
public:
    HistogramGroup(std::shared_ptr<DepthImage> img);

    explicit HistogramGroup(const ImageGroup& other);

    virtual void merge(ImageGroup&) override;

    virtual double similarityTo(ImageGroup&) override;

    /**
     * A factory which allows you to make HistogramGroups, given a dataset
     * and a single image.
     */
    class Factory : public ImageGroup::Factory {
    public:
        virtual ImageGroup* create(std::shared_ptr<DepthImage> img) override;
    };

    const Histogram& getHistogram() const {
        return hist;
    }
protected:
    Histogram hist;

};
#endif
