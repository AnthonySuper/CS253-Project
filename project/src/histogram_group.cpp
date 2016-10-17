#include <histogram_group.hpp>

ImageGroup* HistogramGroup::Factory::create(Dataset ds, unsigned int idx) {
    return new HistogramGroup(ds, idx);
}

HistogramGroup::HistogramGroup(Dataset ds, unsigned int idx) :
    hist(ds->at(idx).getHistogram()) {
        dataset = ds;
        indexes = {idx};
}

HistogramGroup::HistogramGroup(const ImageGroup& o) {
    if(o.getIndexes().size() == 0) {

    }
    else {
        indexes = o.getIndexes();
        Histogram tmp = dataset->at(0).getHistogram();
        for(int i = 1; i < o.getIndexes().size(); i++) {
            tmp = Histogram(tmp, dataset->at(i).getHistogram());
        }
        hist = tmp;
    }
}

void HistogramGroup::merge(ImageGroup &other) {
    std::vector<unsigned int> q = other.getIndexes();
    indexes.insert(indexes.end(),q.begin(), q.end());
    ImageGroup &ig = *this;
    if(typeid(ig) == typeid(other)) {
        HistogramGroup &hs = static_cast<HistogramGroup&>(other);
        hist = Histogram(hist, hs.hist);
    }
    else {
        Histogram tmp(dataset->at(0).getHistogram());
        for(int i = 1; i < indexes.size(); i++) {
            tmp = Histogram(tmp, dataset->at(i).getHistogram());
        }
        hist = tmp;
    }
}

double HistogramGroup::similarityTo(ImageGroup &other) {
    ImageGroup &ig = *this;
    if(typeid(ig) == typeid(other)) {
        HistogramGroup ho = static_cast<HistogramGroup&>(other);
        return ho.hist.minimumSum(hist);
    }
    else {
        HistogramGroup o = HistogramGroup(other);
        return o.hist.minimumSum(hist);
    }
}
