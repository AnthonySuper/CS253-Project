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
    resetSimilarity();
    auto otherIndexes = other.getIndexes();
    for(auto i: otherIndexes) {
        indexes.emplace_back(i);
    }
    hist = Histogram(hist, other.getHistogram());
}

void ImageGrouper::Group::compareSimilarity(int i, double sim) {
    if(sim > nearestSimilarity) {
        nearestSimilarity = sim;
        nearestIndex = i;
    }
}

ImageGrouper::ImageGrouper(std::shared_ptr<ImageDataset> ds) :
  dataset(ds) {
    assert(ds != nullptr);
    groups.reserve(dataset->size());
    for(unsigned int i = 0; i < dataset->size(); ++i) {
        groups.emplace_back(i, dataset);
    }
}

void ImageGrouper::reduceToGroupCount(int count) {
    if((unsigned int) count > groups.size() || count <= 0) {
        throw std::invalid_argument("Count is too large or negative");
    }
    calculateNearestNeighbors();
    while(groups.size() > (unsigned int) count) {
        mergeClosetGroups();
    }
}

void ImageGrouper::mergeGroups(int first, int second) {
    auto &firstGroup = groups.at(first);
    auto &secondGroup = groups.at(second);
    firstGroup.merge(secondGroup);
    groups.erase(groups.begin() + second);
    // Now we must recalcuate some things
    // For one, the indexes after second have all moved to the left one
    // So let's move them
    for(auto &g: groups) {
        if(g.nearestIndex > second) {
            g.nearestIndex -= 1;
        }
    }
    // Now move some other crap over
    for(unsigned int i = 0; i < groups.size(); ++i) {
        auto& group = groups.at(i);
        if(group.nearestIndex == first ||
                group.nearestIndex == second) {
            group.resetSimilarity();
            calculateNearestNeighbor(i);
        }
    }
    calculateNearestNeighbor(first);
}

void ImageGrouper::mergeClosetGroups() {
    int closestIndex = getClosestGroupIndex();
    auto group = groups[closestIndex];
    auto first = std::min(closestIndex, group.nearestIndex);
    auto second = std::max(closestIndex, group.nearestIndex);
    mergeGroups(first, second);
}

int ImageGrouper::getClosestGroupIndex() {
    double largestSim = -1;
    int currentClosest = 0;
    for(unsigned int i = 0; i < groups.size(); ++i) {
        auto g = groups[i];
        if(g.nearestSimilarity > largestSim) {
            largestSim = g.nearestSimilarity;
            currentClosest = i;
        }
    }
    return currentClosest;
}

void ImageGrouper::calculateNearestNeighbors() {
    for(auto& g: groups) {
        g.resetSimilarity();
    }
    for(unsigned int i = 0; i < groups.size(); ++i) {
        calculateNearestNeighbor(i, i + 1);
    }
}


void ImageGrouper::calculateNearestNeighbor(int base, int start) {
    auto& group = groups.at(base);
    for(unsigned int i = static_cast<unsigned int>(start); 
            i < groups.size();
            ++i) {
        // Don't compare against ourselves or our current nearest
        if(static_cast<unsigned int>(group.nearestIndex) == i
                || static_cast<unsigned int>(base) == i) {
            continue;
        }
        auto &group2 = groups.at(i);
        double sim = group.getHistogram().minimumSum(group2.getHistogram());
        group.compareSimilarity(i, sim);
        group2.compareSimilarity(base, sim);
    }
}
