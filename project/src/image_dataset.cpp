#include <image_dataset.hpp>

std::shared_ptr<ImageDataset> ImageDataset::fromFile(std::string fname) {
    ImageDataset *p = new ImageDataset();
    std::shared_ptr<ImageDataset> ds(p);
    std::ifstream f{fname};
    if(! f.is_open()) {
        throw FileNotFoundError(fname);
    }
    std::string s;
    while(f.is_open() && f >> s) {
        ds->emplace_back(s);
    }
    return ds;
}

void ImageDataset::emplace_back(std::string fname) {
    images.emplace_back(fname);
}

const DepthImage& ImageDataset::at(int index) {
    return images.at(index);
}
