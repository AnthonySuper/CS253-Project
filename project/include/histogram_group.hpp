#ifndef _HISTOGRAM_GROUP_HPP
#define _HISTOGRAM_GROUP_HPP
#include <image_group.hpp>
#include <histogram.hpp>

class HistogramGroup : public ImageGroup {
public:
    HistogramGroup(std::shared_ptr<DepthImage> img, int split = 1);

    explicit HistogramGroup(const ImageGroup& other, int split = 1);

    virtual void merge(ImageGroup&) override;

    void merge(HistogramGroup&);

    virtual double similarityTo(ImageGroup&) override;

    double similarityTo(HistogramGroup& ho);

    /**
     * A factory which allows you to make HistogramGroups, given a dataset
     * and a single image.
     */
    class Factory : public ImageGroup::Factory {

    public:
        Factory(int num = 1);
        virtual ImageGroup* create(std::shared_ptr<DepthImage> img) override;
    private:
        const int split;
    };

    const std::vector<Histogram>& getHistogramList() const {
        return histogramList;
    }
protected:
    std::vector<Histogram> splitImage(DepthImage& img);
    const int split;
    std::vector<Histogram> histogramList;

};
#endif
