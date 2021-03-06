#include <image_grouper.hpp>

ImageGrouper::ImageGrouper(ImageDataset &ds, PerceptionTrainer p) :
pt(p)
{
    groups.reserve(ds.size());
    badIndexes.reserve(ds.size());
    size_t size = ds.size();
    for(unsigned int i = 0; i < size; ++i) {
        groups.emplace_back(ds.ptrAt(i));
    }
    
}

void ImageGrouper::reduceToGroupCount(int count)
{
    if((unsigned int) count > groups.size() || count <= 0) {
        throw std::invalid_argument("Count is too large or negative");
    }
    int iterationsReq = groups.size() - count;
    calculateNearestNeighbors();
    int q = 0;
    for(int i = 0; i < iterationsReq; ++i) {
        mergeClosetGroups();
        ++q;
    }
}


void ImageGrouper::mergeGroups(int first,
                               int second)
{
    auto &firstGroup = groups.at(first);
    auto &secondGroup = groups.at(second);
    firstGroup.group.merge(secondGroup.group);
    groups.erase(groups.begin() + second);
    firstGroup.resetSimilarity();
    badIndexes.clear();
    for(unsigned int i = 0; i < groups.size(); ++i) {
        auto &g = groups.at(i);
        if(g.nearestIndex == first || g.nearestIndex >= second) {
            g.resetSimilarity();
            badIndexes.push_back(i);
        }
    }
    badIndexes.push_back(first);
    for(auto i: badIndexes) {
        auto& g1 = groups.at(i);
        for(unsigned int k = i + 1; k < groups.size(); ++k){
            if(k == i) {
                continue;
            }
            auto &g2 = groups.at(k);
            double sim = g1.group.similarityTo(g2.group, pt);
            g1.compareSimilarity(k, sim);
            g2.compareSimilarity(i, sim);
        }
    }
    
}


const std::vector<ImageGroup> ImageGrouper::getGroups() const {
    std::vector<ImageGroup> tmp;
    tmp.reserve(groups.size());
    for(auto g: groups) {
        tmp.emplace_back(g.group);
    }
    return tmp;
}

void ImageGrouper::mergeClosetGroups() {
    int closestIndex = getClosestGroupIndex();
    auto& group = groups[closestIndex];
    auto first = std::min(closestIndex, group.nearestIndex);
    auto second = std::max(closestIndex, group.nearestIndex);
    mergeGroups(first, second);
}


int ImageGrouper::getClosestGroupIndex() {
    double largestSim = -1;
    int currentClosest = 0;
    for(unsigned int i = 0; i < groups.size(); ++i) {
        auto &g = groups[i];
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

void ImageGrouper::calculateNearestNeighbor(int base,
                                            int searchStart)
{
    auto& group = groups.at(base);
    for(unsigned int i = static_cast<unsigned int>(searchStart); 
            i < groups.size();
            ++i) {
        // Don't compare against ourselves or our current nearest
        if(static_cast<unsigned int>(group.nearestIndex) == i
                || static_cast<unsigned int>(base) == i) {
            continue;
        }
        auto &group2 = groups.at(i);
        double similarity = group.group.similarityTo(group2.group, pt);
        group.compareSimilarity(i, similarity);
        group2.compareSimilarity(base, similarity);
    }
}



std::ostream& operator<<(std::ostream& os, const ImageGrouper &g) {
    for(auto& group: g.groups) {
        os << group.group << std::endl << std::endl;
    }
    return os;
}
