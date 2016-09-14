#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <pgm_image.hpp>
#include <depth_image.hpp>

int main(int argc, char *argv[]) {
    if(argc < 3) {
        std::cerr << "Not enough filenames given" << std::endl;
        return -1;
    }
    try {
        auto first = PgmImage(argv[1]);
        auto second = PgmImage(argv[2]);
        auto firstImage = DepthImage(first);
        auto secondImage = DepthImage(second);
        auto compared = firstImage.compareTo(secondImage);
        std::cout << std::get<0>(compared) << '\t' << std::get<1>(compared);
        std::cout << std::endl;
        return 0;
    }
    catch(const std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}
