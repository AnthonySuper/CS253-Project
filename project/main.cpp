#include <iostream>
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
        auto neighbors = d.calculateNearestNeighbors();
        for(auto q : neighbors) {
            auto nearestIndex = q.getNearestNeighborIndex();
            auto nearest = neighbors[nearestIndex];
            auto error = q.getNearestNeighborSimilarity();
            std::cout << q.filename << "\t";
            std::cout << nearest.filename << "\t";
            std::cout << error << std::endl;
        }
        return 0;
    }
    catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}
