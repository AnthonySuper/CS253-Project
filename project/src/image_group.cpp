#include <image_group.hpp>

std::ostream& operator<<(std::ostream& os, const ImageGroup& g) {
    for(auto i: g.indexes) {
        os << g.dataset->at(i).getFileName() << '\t';
    }
    return os;
}

ImageGroup::operator std::string() const {
    std::stringstream ss;
    ss << *this;
    return ss.str();
}

ImageGroup::~ImageGroup() {
}
