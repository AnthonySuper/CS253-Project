#include <image_grouper.hpp>

ImageGrouper::Group::Group(unsigned int index, shared_ptr<ImageDataset> ds) :
    ds(ds)
{
    assert(ds != nullptr);
    indexes.emplace_back(index);
    hist = ds->at(index).getHistogram();
}

ImageGrouper::Group::operator std::string() const {
    std::stringstream ss;
    for(auto i: indexes) {
        ss << ds->at(i).getFileName() << '\t';
    }
    return ss.str();
}

ImageGrouper::operator std::string() const {
    std::stringstream ss;
    ss << *this;
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const ImageGrouper& ig) {
    for(auto &g: ig.groups) {
        os << g << std::endl;
    }
    return os;
}

ImageGrouper::Group::Group(const Group& g1, const Group& g2) :
    ds(g1.getDataset()),
    hist(g1.getHistogram(), g2.getHistogram())
{
    assert(g1.getDataset() == g2.getDataset());
    assert(ds != nullptr);
    auto g1in = g1.getIndexes();
    auto g2in = g2.getIndexes();
    for(auto i: g1in) {
        indexes.emplace_back(i);
    }
    for(auto i: g2in) {
        indexes.emplace_back(i);;
    }
}

void ImageGrouper::Group::merge(const Group& other) {
    auto otherIndexes = other.getIndexes();
    for(auto i: otherIndexes) {
        indexes.emplace_back(i);
    }
    hist = Histogram(hist, other.getHistogram());
}

ImageGrouper::ImageGrouper(std::shared_ptr<ImageDataset> ds) :
  dataset(ds) {
    assert(ds != nullptr);
    groups.reserve(dataset->size());
    for(int i = 0; i < dataset->size(); ++i) {
        groups.emplace_back(i, dataset);
    }
}

void ImageGrouper::reduceToGroupCount(int count) {
    if(groupInfo.size() == 0) {
        calculateNearestNeighbors();
    }
    while(groups.size() > count) {
        mergeClosetGroups();
    }
}

void ImageGrouper::mergeGroups(int first, int second) {
    auto &firstGroup = groups.at(first);
    auto &secondGroup = groups.at(second);
    firstGroup.merge(secondGroup);
    groups.erase(groups.begin() + second);
    groupInfo.erase(groupInfo.begin() + second);
    // Now we must recalcuate some things
    // For one, the indexes after second have all moved to the left one
    // So let's move them
    for(auto info: groupInfo) {
        if(info.nearestNeighborIndex > second) {
            info.nearestNeighborIndex -= 1;
        }
    }
    // Now move some other crap over
    for(unsigned int i = 0; i < groupInfo.size(); ++i) {
        auto& info = groupInfo.at(i);
        if(info.nearestNeighborIndex == first ||
                info.nearestNeighborIndex == second) {
            info.reset();
            calculateNearestNeighbor(i);
        }
    }
    groupInfo.at(first).reset();
    calculateNearestNeighbor(first);
}

void ImageGrouper::mergeClosetGroups() {
    int closest = getClosestGroupIndex();
    auto info = groupInfo[closest];
    auto first = std::min(closest, info.nearestNeighborIndex);
    auto second = std::max(closest, info.nearestNeighborIndex);
    mergeGroups(first, second);
}

int ImageGrouper::getClosestGroupIndex() {
    double largestSim = -1;
    int currentClosest = 0;
    for(int i = 0; i < groupInfo.size(); ++i) {
        auto g = groupInfo[i];
        if(g.nearestNeighborSimilarity > largestSim) {
            largestSim = g.nearestNeighborSimilarity;
            currentClosest = i;
        }
    }
    return currentClosest;
}

void ImageGrouper::calculateNearestNeighbors() {
    groupInfo.resize(groups.size());
    for(int i = 0; i < groupInfo.size(); ++i) {
        calculateNearestNeighbor(i, i + 1);
    }
}


void ImageGrouper::calculateNearestNeighbor(int base, int start) {
    auto& info = groupInfo.at(base);
    const auto &group = groups.at(base);
    for(unsigned int i = static_cast<unsigned int>(start); 
            i < groupInfo.size(); 
            ++i) {
        // Don't bother to re-check histograms against our current nearest
        if(info.nearestNeighborIndex == static_cast<int>(i)) {
            continue;
        }
        // dont' check against ourselves
        if(base == i) {
            continue;
        }
        // get secondary info
        auto &info2 = groupInfo.at(i);
        // get secondary group
        const auto &group2 = groups.at(i);
        double sim = group.getHistogram().minimumSum(group2.getHistogram());
        // if we are closer to this group
        if(sim > info.nearestNeighborSimilarity) {
            info.nearestNeighborIndex = i;
            info.nearestNeighborSimilarity = sim;
        }
        // if they are closer to this group
        if(sim > info2.nearestNeighborSimilarity) {
            info2.nearestNeighborIndex = base;
            info2.nearestNeighborSimilarity = sim;
        }
    }
}
