#include <depth_image_list.hpp>
DepthImageList DepthImageList::fromFile(std::string filename) {
    std::ifstream f{filename};
    if(! f.is_open()) {
        throw FileNotFoundError(filename);
    }
    DepthImageList list;
    std::string fname("");
    while(! f.eof() && f >> fname) {
      list.list.emplace_back(fname);
    }
    return list;
}

DepthImageListItem::DepthImageListItem(std::string n) :
    img(n),
    filename(n)
{}

DepthImageListItem::DepthImageListItem(DepthImage i, std::string fname) :
    img(i),
    filename(fname)
{}

const std::vector<DepthImageList::Item>& 
DepthImageList::calculateNearestNeighbors() {
    for(unsigned int i = 0; i < list.size(); ++i) {
        auto& current = list.at(i);
        for(unsigned int j = i + 1; j < list.size(); ++j) {
            auto& test = list.at(j);
            if(test.nearestNeighborIndex == static_cast<int>(i)) {
                continue;
            }
            double sum = current.img.minimumSumComparison(test.img);
            if(sum > current.nearestNeighborSimilarity) {
                current.nearestNeighborIndex = j;
                current.nearestNeighborSimilarity = sum;
            }
            if(sum > test.nearestNeighborSimilarity) {
                test.nearestNeighborIndex = i;
                test.nearestNeighborSimilarity = sum;
            }
        }
    }
    return list;
}



