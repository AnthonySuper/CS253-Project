#include <perception_trainer.hpp>

PerceptionTrainer::PerceptionTrainer(ImageDataset _ds, int _klass) :
    ds{_ds}, klass{_klass}, p{}
{}


void PerceptionTrainer::trainUntilEpoch(int c) {
    for(int i = 0; i < c; ++i) {
        for(auto img: ds) {
            int c = ((img->getCategory() == klass) ? 1 : -1);
            p.train(img->getHistogram(), c);
        }
    }
}
