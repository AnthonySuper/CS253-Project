#ifndef _PERCEPTION_TRAINER_HPP
#define _PERCEPTION_TRAINER_HPP
#include <perception.hpp>
#include <image_dataset.hpp>
#include <vector>
#include <thread>
#include <future>

class PerceptionTrainer {
public:
    PerceptionTrainer(ImageDataset &ds, int count = 100);

    std::vector<Perception> getPerceptions() { return perceptions; }
    
    const std::vector<Perception>& getPerceptions() const { return perceptions; }

private:
    
    std::vector<Perception> perceptions;
};
#endif
