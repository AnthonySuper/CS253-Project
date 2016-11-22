#include <perception_trainer.hpp>

static Perception trainPerception(const ImageDataset& ds, int itrCount, int klass) {
    Perception p;
    for(int i = 0; i < itrCount; ++i) {
        for(auto img: ds) {
            int c = ((img->getCategory() == klass) ? 1 : -1);
            p.train(img->getHistogram(), c);
        }
    }
    return p;
}



PerceptionTrainer::PerceptionTrainer(ImageDataset _ds, int count) :
    ds{_ds}
{
    std::vector<std::future<Perception>> futures;
    futures.reserve(ds.getClassList().size());
    for(auto klass: ds.getClassList()) {
        futures.emplace_back(std::async([=, &_ds](int klass){
            return trainPerception(ds, count, klass);
        }, klass));
    }
    for(auto& ft: futures) {
        perceptions.emplace_back(ft.get());
    }
}


