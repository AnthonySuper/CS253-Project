#include <perception_trainer.hpp>

static Perception trainPerception(const ImageDataset& ds, int itrCount, int klass) {
    Perception p;
    for(int i = 0; i < itrCount; ++i) {
        for(int j = 0; j < ds.size(); ++j) {
            const auto& img = ds.refAt(j);
            int c = ((img.getCategory() == klass) ? 1 : -1);
            p.train(img.getHistogram(), c);
        }
    }
    return p;
}

PerceptionTrainer::PerceptionTrainer(ImageDataset &ds, int count)
{
    auto& cl = ds.getClassList();
    std::vector<std::thread> threads;
    threads.reserve(cl.size());
    std::mutex dsmut;
    auto k = ds.getClassList();
    for(auto klass: k) {
        threads.emplace_back([=, &ds, &dsmut](int klass){
            auto q = trainPerception(ds, count, klass);
            std::lock_guard<std::mutex> lock(dsmut);
            perceptions.emplace_back(std::move(q));
        }, klass);
    }
    for(auto& t: threads) {
        t.join();
    }
}


