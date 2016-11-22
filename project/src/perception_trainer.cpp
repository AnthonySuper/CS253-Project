#include <perception_trainer.hpp>

PerceptionTrainer::PerceptionTrainer(ImageDataset _ds, int count) :
    ds{_ds}, perceptions(ds.getClassList().size())
{
    //std::vector<std::thread> threads;
    const auto& cl = ds.getClassList();
    int idx = 0;
    for(auto klass: cl) {
        trainPerception(count, klass, idx);
        /*
         threads.emplace_back(&PerceptionTrainer::trainPerception,
         this,
         c,
         klass,
         idx);
         */
    }
    /*
     for(auto& t: threads) {
     t.join();
     }
     */
}

void PerceptionTrainer::trainPerception(int count,
                                        int klass,
                                        int idx)
{
    Perception& p = perceptions.at(idx);
    for(int i = 0; i < count; ++i) {
        for(auto img: ds) {
            int c = ((img->getCategory() == klass) ? 1 : -1);
            p.train(img->getHistogram(), c);
        }
    }
}
