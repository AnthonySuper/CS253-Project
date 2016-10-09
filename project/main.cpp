#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <depth_image.hpp>
#include <image_dataset.hpp>

int main(int argc, char *argv[]) {
    if(argc < 2) {
        std::cerr << "Not enough filenames given" << std::endl;
        return -1;
    }
    try {
        auto d = ImageDataset::fromFile(argv[1]);

    }
    catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}
