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

ImageGroup::~ImageGroup() {
}

ImageGroup::Factory::~Factory() {

}
