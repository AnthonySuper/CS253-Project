#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <depth_image.hpp>
#include <image_dataset.hpp>
#include <image_grouper.hpp>
#include <histogram_group.hpp>

using std::cout;
using std::endl;
int main(int argc, char *argv[]) {
    if(argc < 3) {
        std::cerr << "Not enough filenames given" << std::endl;
        return -1;
    }
    try {
        HistogramGroup::Factory fac;
        auto d = ImageDataset::fromFile(argv[1]);
        int i = std::stoi(argv[2]);
        auto groups = ImageGrouper(d, &fac);
        groups.reduceToGroupCount(i);
        std::cout << groups << std::endl;
    }
    catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
        std::cerr << typeid(e).name() << std::endl;
        return -1;
    }
}
