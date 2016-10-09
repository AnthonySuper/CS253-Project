#ifndef _IMAGE_GROUPER_HPP_
#define _IMAGE_GROUPER_HPP_
#include <memory>
#include <vector>
#include <image_dataset.hpp>
#include <errors.hpp>
#include <cassert>
#include <tuple>
#include <histogram.hpp>

using std::shared_ptr;
using std::vector;

class ImageGrouper {
public:
    ImageGrouper(shared_ptr<ImageDataset>);
    void reduceToGroupCount(int count);

    class Group {
    public:
        Group(unsigned int index, shared_ptr<ImageDataset>);
        Group(const Group& g1, const Group& g2);

        void merge(const Group& other);

        inline const vector<unsigned int>& getIndexes() const {
            return indexes;
        }

        inline const shared_ptr<ImageDataset> getDataset() const {
            return ds;
        }

        inline const Histogram& getHistogram() const {
            return hist;
        }

    protected:
        vector<unsigned int> indexes;
        shared_ptr<ImageDataset> ds;
        Histogram hist;
    };

    inline const std::vector<Group>& getGroups() {
        return groups;
    }
protected:
    struct GroupHelper {
        int nearestNeighborIndex = -1;
        double nearestNeighborSimilarity = -1;
        inline void reset() {
            nearestNeighborIndex = -1;
            nearestNeighborSimilarity = -1;
        }
    };
    void mergeGroups(int first, int second);
    int getClosestGroupIndex();
    void calculateNearestNeighbor(int baseIndex, int startingIndex = 0);
    void calculateNearestNeighbors();
    void mergeClosetGroups();
    vector<GroupHelper> groupInfo;
    vector<Group> groups;
    shared_ptr<ImageDataset> dataset;
};

#endif
