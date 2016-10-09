#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <depth_image.hpp>
#include <image_dataset.hpp>
#include <image_grouper.hpp>

int main(int argc, char *argv[]) {
    if(argc < 3) {
        std::cerr << "Not enough filenames given" << std::endl;
        return -1;
    }
    try {
        auto d = ImageDataset::fromFile(argv[1]);
        int i = std::stoi(argv[2]);
        if(i < 1 || i > d->size()) {
            throw std::runtime_error{"Cannot make that many groups"};
        }
        auto groups = ImageGrouper(d);
        groups.reduceToGroupCount(i);
        std::cout << groups << std::endl;
    }
    catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}
