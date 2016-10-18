#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <depth_image.hpp>
#include <image_dataset.hpp>
#include <image_grouper.hpp>
#include <histogram_group.hpp>
#include <quad_group.hpp>

ImageGroup::Factory* getFactory(int idx) {
    if(idx == 1) {
        return new HistogramGroup::Factory();
    }
    else if(idx == 2) {
        return new QuadGroup::Factory();
    }
    else {
        throw std::runtime_error("Index out of bounds!");
    }
}

using std::cout;
using std::endl;
int main(int argc, char *argv[]) {
    if(argc < 4) {
        std::cerr << "Not enough arguments" << std::endl;
        return -1;
    }
    try {
        auto d = ImageDataset::fromFile(argv[1]);
        int i = std::stoi(argv[2]);
        ImageGroup::Factory *f = getFactory(std::stoi(argv[3]));
        auto groups = ImageGrouper(d, f);
        groups.reduceToGroupCount(i);
        std::cout << groups << std::endl;
        delete f;
    }
    catch(std::string &s) {
    }
    /*
    catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
        std::cerr << typeid(e).name() << std::endl;
        return -1;
    }
    */
}
