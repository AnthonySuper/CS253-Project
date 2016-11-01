#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <depth_image.hpp>
#include <image_dataset.hpp>
#include <perception_trainer.hpp>


using std::cout;
using std::endl;
int main(int argc, char *argv[]) {
    if(argc < 3) {
        std::cerr << "Not enough arguments" << std::endl;
        return -1;
    }
    try {
        auto d = ImageDataset::fromFile(argv[1]);
        int i = std::stoi(argv[2]);
        if(! d.hasClass(i)) {
            throw std::runtime_error("Nonexistant image class specified");
        }
        /*
        Perception p;
        p.train(d.at(0)->getHistogram(), 1);
        cout << p << endl;
        cout << p.getValue(d.at(0)->getHistogram());
        */
        
        
        PerceptionTrainer ps(d, i);
        ps.trainUntilEpoch();
        cout << ps.getPerception() << endl;
        
        
    }
    catch(const std::exception &e) {
        std::cerr << typeid(e).name() << " " << e.what() << std::endl;
        return -1;
    }
    
}
