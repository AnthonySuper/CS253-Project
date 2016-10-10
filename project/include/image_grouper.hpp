#ifndef _IMAGE_GROUPER_HPP_
#define _IMAGE_GROUPER_HPP_
#include <memory>
#include <vector>
#include <image_dataset.hpp>
#include <errors.hpp>
#include <cassert>
#include <sstream>
#include <tuple>
#include <histogram.hpp>

using std::shared_ptr;
using std::vector;

class ImageGrouper {
public:
    ImageGrouper(shared_ptr<ImageDataset>);
    void reduceToGroupCount(int count);

    explicit operator std::string() const;
    friend std::ostream& operator<<(std::ostream& os, const ImageGrouper&);

    class Group {
    public:
        Group(unsigned int index, shared_ptr<ImageDataset>);
        Group(const Group& g1, const Group& g2);

        void merge(const Group& other);

        explicit operator std::string() const;

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
        inline void resetSimilarity() {
            nearestIndex = -1;
            nearestSimilarity = -1;
        }
        void compareSimilarity(int index, double sim);
        vector<unsigned int> indexes;
        shared_ptr<ImageDataset> ds;
        Histogram hist;
        int nearestIndex = -1;
        double nearestSimilarity = -1;
        // this is a friend because it needs to see certain parts of
        // our internal representation. 
        friend class ImageGrouper;
    };

    inline const std::vector<Group>& getGroups() {
        return groups;
    }
protected:
    void mergeGroups(int first, int second);
    int getClosestGroupIndex();
    void calculateNearestNeighbor(int baseIndex, int startingIndex = 0);
    void calculateNearestNeighbors();
    void mergeClosetGroups();
    vector<Group> groups;
    shared_ptr<ImageDataset> dataset;
};

inline std::ostream& operator<<(std::ostream& os, 
        const ImageGrouper::Group &g) {
    return os << (std::string) g;
}

#endif
