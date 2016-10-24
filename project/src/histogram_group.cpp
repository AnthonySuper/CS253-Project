#include <histogram_group.hpp>

static std::vector<Histogram> mergeHistVecs(std::vector<Histogram> a,
        std::vector<Histogram> b) {
    if(a.size() != b.size() || a.size() == 0) {
        throw std::invalid_argument("List lengths are unequal or invalid");
    }
    std::vector<Histogram> tmp;
    tmp.reserve(a.size());
    for(int i = 0; i < a.size(); ++i) {
        tmp.emplace_back(a[i], b[i]);
    }
    return tmp;
}

HistogramGroup::Factory::Factory(int _split) :
    split(_split)
{}

ImageGroup* HistogramGroup::Factory::create(std::shared_ptr<DepthImage> i) {
    return new HistogramGroup(i, split);
}

HistogramGroup::HistogramGroup(std::shared_ptr<DepthImage> img, int _split) :
    split(_split)
{
    if(img == nullptr) {
        throw std::invalid_argument("Cannot pass a nullptr to an imagegroup");
    }
    if(split < 0 || split > 128) {
        throw std::invalid_argument("Split is too great");
    }
    images = {img};
    histogramList = splitImage(*img);
}

HistogramGroup::HistogramGroup(const ImageGroup& o, int _split) :
  split(_split) 
{
    if(o.getImages().size() == 0) {
        throw std::runtime_error("Can't have zero-length groups!");
    }
    else {
        images = o.getImages();
        std::vector<Histogram> tmp = splitImage(*images.at(0));
        for(unsigned int i = 1; i < images.size(); i++) {
            std::vector<Histogram> tmp2 = splitImage(*images.at(i));
            tmp = mergeHistVecs(tmp, tmp2);
        }
        histogramList = tmp;
    }
}

std::vector<Histogram> HistogramGroup::splitImage(DepthImage& img) {
    std::vector<Histogram> r;
    int div = (128 / split);
    for(int x = 0; (x + div) <= 128; x += div) {
        for(int y = 0; (y + div) <= 128; y += div) {
            r.emplace_back(img.getSection(x, y, div - 1, div - 1));
        }
    }
    return r;
}

void HistogramGroup::merge(ImageGroup &other) {
    auto imgs = other.getImages();
    appendImages(imgs);
    ImageGroup &ig = *this;
    if(typeid(ig) == typeid(other)) {
        HistogramGroup &hs = static_cast<HistogramGroup&>(other);
        if(hs.split != split) {
            throw std::invalid_argument("Groups do not have same size!");
        }
        merge(hs);
    }
    else {
        HistogramGroup hs = HistogramGroup(other, split);;
        merge(hs);
    }
}

void HistogramGroup::merge(HistogramGroup& o) {
    histogramList = mergeHistVecs(histogramList, o.histogramList);
}

double HistogramGroup::similarityTo(ImageGroup &other) {
    ImageGroup &ig = *this;
    if(typeid(ig) == typeid(other)) {
        HistogramGroup ho = static_cast<HistogramGroup&>(other);
        return similarityTo(ho);
    }
    else {
        HistogramGroup o = HistogramGroup(other);
        return similarityTo(o);
    }
}

double HistogramGroup::similarityTo(HistogramGroup& ho) {
    if(ho.split != split) {
        throw std::invalid_argument("histogram groups have different splits");
    }
    double sum = 0;
    for(int i = 0; i < histogramList.size(); ++i) {
        sum += histogramList[i].minimumSum(ho.histogramList[i]);
    }
    return sum / (((double) split) * ((double) split));
}
