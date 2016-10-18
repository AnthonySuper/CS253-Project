#include <histogram_group.hpp>

ImageGroup* HistogramGroup::Factory::create(std::shared_ptr<DepthImage> i) {
    return new HistogramGroup(i);
}

HistogramGroup::HistogramGroup(std::shared_ptr<DepthImage> img)
{
    if(img == nullptr) {
        throw std::invalid_argument("Cannot pass a nullptr to an imagegroup");
    }
    images = {img};
    hist = img->getHistogram();
}

HistogramGroup::HistogramGroup(const ImageGroup& o) {
    if(o.getImages().size() == 0) {

    }
    else {
        images = o.getImages();
        Histogram tmp = images.at(0)->getHistogram();
        for(int i = 1; i < images.size(); i++) {
            tmp = Histogram(tmp, images.at(i)->getHistogram());
        }
        hist = tmp;
    }
}

void HistogramGroup::merge(ImageGroup &other) {
    auto imgs = other.getImages();
    images.insert(images.end(), imgs.begin(), imgs.end());
    ImageGroup &ig = *this;
    if(typeid(ig) == typeid(other)) {
        HistogramGroup &hs = static_cast<HistogramGroup&>(other);
        hist = Histogram(hist, hs.hist);
    }
    else {
        Histogram tmp(imgs.at(0)->getHistogram());
        for(int i = 1; i < imgs.size(); i++) {
            tmp = Histogram(tmp, imgs.at(i)->getHistogram());
        }
        hist = tmp;
    }
}

double HistogramGroup::similarityTo(ImageGroup &other) {
    ImageGroup &ig = *this;
    if(typeid(ig) == typeid(other)) {
        HistogramGroup ho = static_cast<HistogramGroup&>(other);
        return ho.hist.minimumSum(hist);
    }
    else {
        HistogramGroup o = HistogramGroup(other);
        return o.hist.minimumSum(hist);
    }
}
