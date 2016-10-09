#ifndef _DEPTH_IMAGE_LIST_HPP
#define _DEPTH_IMAGE_LIST_HPP
#include <fstream>
#include <vector>
#include <depth_image.hpp>
#include <limits>

class DepthImageList;

class DepthImageListItem {
    friend class DepthImageList;
    public:
        DepthImageListItem(std::string fname);
        DepthImageListItem(DepthImage i, std::string fname);
        DepthImage img;
        const std::string filename;
        inline int getNearestNeighborIndex() const {
            return nearestNeighborIndex;
        }
        inline double getNearestNeighborSimilarity() const {
            return nearestNeighborSimilarity;
        }
    protected:
        int nearestNeighborIndex = -1;
        double nearestNeighborSimilarity = std::numeric_limits<double>::min();
};

class DepthImageList {
public:
    static DepthImageList fromFile(std::string filename);
    using Item = DepthImageListItem;
    void insert(DepthImage i, std::string fname);
    const std::vector<Item>& calculateNearestNeighbors();
protected:
    std::vector<Item> list;
};

#endif
