#include <depth_image_list.hpp>

DepthImageList DepthImageList::fromFile(std::string filename) {
    std::ifstream f{filename};
    if(! f.is_open()) {
        throw FileNotFoundError(filename);
    }
    DepthImageList list;
    std::string fname("");
    while(! f.eof() && f >> fname) {
        PgmImage i{fname};
        list.list.emplace_back(std::move(i), fname);
    }
    return list;
}

const std::vector<DepthImageList::Item>& 
DepthImageList::calculateNearestNeighbors() {
    for(int i = 0; i < list.size(); ++i) {
        auto& current = list[i];
        for(int j = i + 1; j < list.size(); ++j) {
            auto& test = list[j];
            if(test.nearestNeighborIndex == i) {
                continue;
            }
            double sum = current.img.minimumSumComparison(test.img);
            if(sum < current.nearestNeighborDistance) {
                current.nearestNeighborIndex = j;
                current.nearestNeighborDistance = sum;
            }
            if(sum < test.nearestNeighborDistance) {
                test.nearestNeighborIndex = i;
                test.nearestNeighborDistance = sum;
            }
        }
    }
    return list;
}


DepthImageListItem::DepthImageListItem(DepthImage i, std::string fname) :
    img(i),
    filename(fname)
{}

DepthImageListItem::DepthImageListItem(const PgmImage&& r, std::string fname) :
    img(r),
    filename(fname)
{}
