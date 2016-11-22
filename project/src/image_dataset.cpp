#include <image_dataset.hpp>

ImageDataset ImageDataset::fromFile(std::string fname) {
    ImageDataset ds;
    std::ifstream f{fname};
    std::vector<std::future<std::shared_ptr<DepthImage>>> futures;
    if(! f.is_open()) {
        throw FileNotFoundError(fname);
    }
    std::string s;
    while(f.is_open() && f >> s) {
        futures.emplace_back(std::async([=](std::string s) {
            return std::make_shared<DepthImage>(s);
        }, s));
    }
    for(auto &f: futures) {
        ds.emplace_back(f.get());
    }
    return ds;
}

void ImageDataset::emplace_back(ImagePtr ip) {
    images.emplace_back(ip);
    addClass(ip->getCategory());
}

void ImageDataset::emplace_back(std::string fname) {
    images.emplace_back(std::make_shared<DepthImage>(fname));
    addClass(images.at(images.size() - 1)->getCategory());
}

void ImageDataset::emplace_back(const DepthImage& img) {
    images.emplace_back(std::make_shared<DepthImage>(img));
    addClass(images.at(images.size() - 1)->getCategory());
}

std::shared_ptr<DepthImage>ImageDataset::at(int index) {
    return images.at(index);
}

void ImageDataset::addClass(int k) {
    classList.insert(k);
}
