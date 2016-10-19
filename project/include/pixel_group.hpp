#ifndef _PIXEL_GROUP_H
#define _PIXEL_GROUP_H
#include <image_group.hpp>

class PixelGroup : public ImageGroup {
public:
    PixelGroup(std::shared_ptr<DepthImage>);

    PixelGroup(ImageGroup&);

    virtual void merge(ImageGroup&) override;

    virtual double similarityTo(ImageGroup&) override;

    class Factory : public ImageGroup::Factory {
    public:
        virtual ImageGroup* create(std::shared_ptr<DepthImage> im);
    };

protected:
    void merge(PixelGroup&);
    double similarityTo(PixelGroup&);
    static constexpr int expected_height = 128;
    static constexpr int expected_width = 128;
    std::vector<double> pixels;
};
#endif
