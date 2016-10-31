#ifndef _PERCEPTION_TRAINER_HPP
#define _PERCEPTION_TRAINER_HPP
#include <perception.hpp>
#include <image_dataset.hpp>

class PerceptionTrainer {
public:
    PerceptionTrainer(ImageDataset ds, int klass);
    void trainUntilEpoch(int count = 100);
    Perception getPerception() {
        return p;
    }
private:
    const ImageDataset ds;
    const int klass;
    Perception p;
};
#endif
