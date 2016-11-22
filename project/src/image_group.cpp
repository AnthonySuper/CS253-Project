#include <image_group.hpp>

std::ostream& operator<<(std::ostream& os, const ImageGroup& g) {
    auto images = g.getImages();
    for(auto img: images) {
        os << img->getFileName() << '\t';
    }
    return os;
}

ImageGroup::operator std::string() const {
    std::stringstream ss;
    ss << *this;
    return ss.str();
}

ImageGroup::ImageGroup(std::shared_ptr<DepthImage> im) :
images{im}, hg{im->getHistogram()}
{
}

double ImageGroup::similarityTo(ImageGroup& o, const PerceptionTrainer& pt)
{
    double sum = 0;
    for(const Perception& p: pt.getPerceptions()) {
        double under = (p.getValue(hg) - p.getValue(o.hg));
        under = under*under + std::numeric_limits<double>::min();
        sum += 1 / under;
    }
    return sum;
}

int ImageGroup::getMostOccuringClassCount() const {
    std::unordered_map<int, int> m;
    for(auto img: images) {
        int cat = img->getCategory();
        auto f = m.find(cat);
        if(f != m.end()) {
            f->second = (f->second + 1);
        }
        else {
            m.insert(std::make_pair(cat, 1));
        }
    }
    auto it = std::max_element(m.begin(), 
            m.end(), 
            [](auto a, auto b) { return a.second < b.second; });
    return it->second;
}

void ImageGroup::merge(ImageGroup& other) {
    hg = Histogram(hg, other.hg);
    images.insert(images.end(),
                  other.images.begin(),
                  other.images.end());
}
