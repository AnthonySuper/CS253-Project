#include <quad_group.hpp>

ImageGroup* QuadGroup::Factory::create(std::shared_ptr<DepthImage> im) {
    return new QuadGroup(im);
}

QuadGroup::QuadGroup(std::shared_ptr<DepthImage> im) {
    auto vec_a = im->getSection(0, 0, 64, 64);
    auto vec_b = im->getSection(63, 0, 64, 64);
    auto vec_c = im->getSection(0, 63, 64, 64);
    auto vec_d = im->getSection(63, 63, 64, 64);
    quad_a = vec_a;
    quad_b = vec_b;
    quad_c = vec_c;
    quad_d = vec_d;
    images = {im};
}

void QuadGroup::merge(ImageGroup& g) {
    appendImages(g.getImages());
    ImageGroup &go = *this;
    if(typeid(go) == typeid(g)) {
        QuadGroup &qg = static_cast<QuadGroup&>(g);
        quad_a = Histogram(quad_a, qg.quad_a);
        quad_b = Histogram(quad_b, qg.quad_b);
        quad_c = Histogram(quad_c, qg.quad_c);
        quad_c = Histogram(quad_d, qg.quad_d);
    }
    else {
        throw std::runtime_error("I'm too lazy to implement this right now");
    }
}

double QuadGroup::similarityTo(ImageGroup& g) {
    ImageGroup &go = *this;
    if(typeid(go) == typeid(g)) {
        QuadGroup& qg = static_cast<QuadGroup&>(g);
        double a = quad_a.minimumSum(qg.quad_a);
        double b = quad_b.minimumSum(qg.quad_b);
        double c = quad_c.minimumSum(qg.quad_c);
        double d = quad_d.minimumSum(qg.quad_d);
        return (a + b + c + d) / 4.0;
    }
    else {
        throw std::runtime_error("I'm too lazy to implement this right now");
        return 0;
    }
}
