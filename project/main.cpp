#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <pgm_image.hpp>
#include <depth_image.hpp>
#include <depth_image_list.hpp>

int main(int argc, char *argv[]) {
    if(argc < 2) {
        std::cerr << "Not enough filenames given" << std::endl;
        return -1;
    }
    try {
        auto d = DepthImageList::fromFile(argv[1]);
        return 0;
    }
    catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}
