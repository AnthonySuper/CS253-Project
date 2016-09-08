#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <histogram.hpp>

int main(int argc, char *argv[]) {
    if(argc < 3) {
        std::cerr << "Not enough filenames given" << std::endl;
        return -1;
    }
    try {
        Histogram first = Histogram::fromFile(argv[1]);
        Histogram second = Histogram::fromFile(argv[2]);
        double r = first.zipNormalized(second);
        std::cout << r << std::endl;
        return 0;
    }
    catch(const std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}
