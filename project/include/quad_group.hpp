#ifndef _QUAD_GROUP_HPP
#define _QUAD_GROUP_HPP
#include <image_group.hpp>

class QuadGroup : public ImageGroup {
public:

    QuadGroup(std::shared_ptr<DepthImage> im);

    virtual void merge(ImageGroup&) override;

    virtual double similarityTo(ImageGroup&) override;

    class Factory : public ImageGroup::Factory {
    public:
        virtual ImageGroup* create(std::shared_ptr<DepthImage>) override;
    };

protected:
    Histogram quad_a;
    Histogram quad_b;
    Histogram quad_c;
    Histogram quad_d;
};

#endif
