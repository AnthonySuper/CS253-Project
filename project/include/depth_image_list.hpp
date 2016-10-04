#ifndef _DEPTH_IMAGE_LIST_HPP
#define _DEPTH_IMAGE_LIST_HPP
#include <fstream>
#include <vector>
#include <depth_image.hpp>
#include <pgm_image.hpp>
#include <limits>

class DepthImageList;

class DepthImageListItem {
    friend class DepthImageList;
    public:
        DepthImageListItem(DepthImage i, std::string fname);
        DepthImageListItem(const PgmImage&&, std::string fname);
        DepthImage img;
        const std::string filename;
        inline int getNearestNeighborIndex() {
            return nearestNeighborIndex;
        }
    protected:
        int nearestNeighborIndex = -1;
        double nearestNeighborDistance = std::numeric_limits<double>::max();
};

class DepthImageList {
public:
    static DepthImageList fromFile(std::string filename);
    using Item = DepthImageListItem;
    const std::vector<Item>& calculateNearestNeighbors();
protected:
    std::vector<Item> list;
};

#endif
