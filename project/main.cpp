#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <depth_image.hpp>
#include <image_dataset.hpp>
#include <image_grouper.hpp>
#include <perception_trainer.hpp>


// 0.423

using std::cout;
using std::endl;
int main(int argc, char *argv[]) {
    
    if(argc < 4) {
        std::cerr << "Not enough arguments" << std::endl;
        return -1;
    }
    try {
        /*
        auto d = ImageDataset::fromFile("test_files/given/perceptron_train/correctfiles.txt");
        auto d2 = ImageDataset::fromFile("test_files/given/perceptron_train/correctfiles.txt");
        */
        auto d = ImageDataset::fromFile(argv[1]);
        auto d2 = ImageDataset::fromFile(argv[2]);
        int gc = std::atoi(argv[3]);
        PerceptionTrainer pt(d);
        ImageGrouper g(d2, pt);
        g.reduceToGroupCount(gc);
        std::cout << g << std::endl;
    }
    catch(const std::exception &e) {
        std::cerr << typeid(e).name() << " " << e.what() << std::endl;
        return -1;
    }
    
}
