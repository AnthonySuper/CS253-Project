#include <pixel_group.hpp>

ImageGroup* PixelGroup::Factory::create(std::shared_ptr<DepthImage> im) {
    return new PixelGroup(im);
}


PixelGroup::PixelGroup(std::shared_ptr<DepthImage> i) {
    images = {i};
    auto imgPixels = i->getPixelData();
    pixels = std::vector<double>(imgPixels.begin(), imgPixels.end());
}

PixelGroup::PixelGroup(ImageGroup& g) {
    auto imgs = g.getImages();
    if(imgs.size() == 0) {
      pixels.reserve(expected_height * expected_width);
    }
    else {
        auto& imgPixels = imgs.at(0)->getPixelData();
        pixels = std::vector<double>(imgPixels.begin(), imgPixels.end());
        for(unsigned int i = 1; i < imgs.size(); ++i) {
            auto& ip = imgs[i]->getPixelData();
            for(unsigned int j = 0; j < pixels.size(); ++j) {
                pixels[j] += ip[j];
            }
        }
        auto totalSize = imgs.size();
        for(unsigned int i = 0; i < pixels.size(); ++i) {
            pixels[i] /= totalSize;
        }
    }
}

void PixelGroup::merge(ImageGroup& o) {
    appendImages(o.getImages());
    ImageGroup& i = *this;
    if(typeid(i) == typeid(o)) {
        PixelGroup& po = static_cast<PixelGroup&>(o);
        merge(po);
    }
    else {
        PixelGroup po(o);
        merge(po);
    }
}

void PixelGroup::merge(PixelGroup& p) {
    if(p.pixels.size() != pixels.size()) {
        throw std::invalid_argument("Sizes do not match");
    }
    auto ourWeight = images.size();
    auto theirWeight = p.images.size();
    auto avgSize = ourWeight + theirWeight;
    std::vector<double> tmp;
    tmp.reserve(expected_height * expected_width);
    auto& otherPixels = p.pixels;
    for(unsigned int i = 0; i < pixels.size(); i++) {
        auto sum = (pixels[i] * ourWeight) + (otherPixels[i] * theirWeight);
        tmp[i] = (sum / avgSize);
    }
}

double PixelGroup::similarityTo(ImageGroup &g) {
    ImageGroup &self = *this;
    if(typeid(self) == typeid(g)) {
        return similarityTo(static_cast<PixelGroup&>(g));
    }
    else {
        PixelGroup pg(g);
        return similarityTo(pg);
    }
}

double PixelGroup::similarityTo(PixelGroup &g) {
    double sum = 0;
    for(unsigned int i = 0; i < pixels.size(); ++i) {
        double diff = pixels[i] - g.pixels[i];
        sum += diff*diff;
    }
    return 1.0/(sum + 1);
}
