#include <image_dataset.hpp>

ImageDataset ImageDataset::fromFile(std::string fname) {
    ImageDataset ds;
    std::ifstream f{fname};
    if(! f.is_open()) {
        throw FileNotFoundError(fname);
    }
    std::string s;
    while(f.is_open() && f >> s) {
        ds.emplace_back(s);
    }
    return ds;
}

void ImageDataset::emplace_back(std::string fname) {
    images.emplace_back(std::make_shared<DepthImage>(fname));
}

void ImageDataset::emplace_back(const DepthImage& img) {
    images.emplace_back(std::make_shared<DepthImage>(img));
}

std::shared_ptr<DepthImage>ImageDataset::at(int index) {
    return images.at(index);
}
