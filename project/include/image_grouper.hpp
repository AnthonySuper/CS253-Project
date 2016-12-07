#ifndef _IMAGE_GROUPER_HPP_
#define _IMAGE_GROUPER_HPP_
#include <memory>
#include <vector>
#include <image_dataset.hpp>
#include <perception_trainer.hpp>
#include <image_group.hpp>
#include <errors.hpp>
#include <cassert>
#include <sstream>
#include <tuple>

using std::shared_ptr;
using std::unique_ptr;
using std::vector;

class ImageGrouper {
public:
    ImageGrouper(ImageDataset&, PerceptionTrainer pt);
    
    void reduceToGroupCount(int count);
    
    friend std::ostream& operator<<(std::ostream &os, const ImageGrouper& g);

    struct GroupHelper {
        int nearestIndex = -1;
        double nearestSimilarity = -1;
        ImageGroup group;
        GroupHelper(ImageGroup ig) : group(ig) {}
        
        inline void resetSimilarity() {
            nearestIndex = -1;
            nearestSimilarity = -1;
        }
        
        inline void compareSimilarity(int indx, double sim) {
            if(sim > nearestSimilarity) {
                nearestSimilarity = sim;
                nearestIndex = indx;
            }
        }
    };

    double getAverageFitness() const;

    const std::vector<ImageGroup> getGroups() const;

protected:
    void mergeGroups(int first, int second);
    int getClosestGroupIndex();
    void compareSimilarity(int start,
                           int end);
    void calculateNearestNeighbor(int baseIndex,
                                  int startingIndex = 0);
    void calculateNearestNeighbors();
    void mergeClosetGroups();
    vector<GroupHelper> groups;
    const PerceptionTrainer pt;
    std::vector<unsigned int> badIndexes;
};

std::ostream& operator<<(std::ostream&,const ImageGroup&);

#endif
