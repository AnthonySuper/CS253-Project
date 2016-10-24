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

void ImageGroup::appendImages(std::vector<ImagePtr> imgs) {
    images.insert(images.end(), imgs.begin(), imgs.end());
}

std::tuple<int, int> ImageGroup::getFitness() const {
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
    return std::make_tuple(it->second, images.size());
}

ImageGroup::~ImageGroup() {
}

ImageGroup::Factory::~Factory() {

}
