#include <image_grouper.hpp>

ImageGrouper::ImageGrouper(ImageDataset ds,
        ImageGroup::Factory *factory)
{
    groups.reserve(ds.size());
    auto images = ds.getImages();
    for(auto img: images) {
        std::shared_ptr<ImageGroup> p(factory->create(img));
        groups.emplace_back(p);
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
    firstGroup.group->merge(*secondGroup.group.get());
    groups.erase(groups.begin() + second);
    std::vector<unsigned int> badIndexes;
    for(unsigned int i = 0; i < groups.size(); ++i) {
        auto &g = groups.at(i);
        if(g.nearestIndex >= 2 || g.nearestIndex == 1) {
            g.resetSimilarity();
            badIndexes.emplace_back(i);
        }
    }
    for(auto i: badIndexes) {
        calculateNearestNeighbor(i);
    }
}


std::vector<std::shared_ptr<ImageGroup>> ImageGrouper::getGroups() const {
    std::vector<std::shared_ptr<ImageGroup>> tmp;
    tmp.reserve(groups.size());
    for(auto g: groups) {
        tmp.emplace_back(g.group);
    }
    return tmp;
}

double ImageGrouper::getAverageFitness() const {
    int max = 0;
    int total = 0;
    for(auto g: groups) {
        auto q = g.group->getMostOccuringClassCount();
        max += q;
        total += g.group->size();
    }
    return (double) max / total;
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

void ImageGrouper::calculateNearestNeighbor(int base, int searchStart) {
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
        double similarity = group.group->similarityTo(*group2.group.get());
        group.compareSimilarity(i, similarity);
        group2.compareSimilarity(base, similarity);
    }
}


std::ostream& operator<<(std::ostream& os, const ImageGrouper &g) {
    for(auto& group: g.groups) {
        os << *(group.group.get()) << std::endl;
    }
    return os;
}
