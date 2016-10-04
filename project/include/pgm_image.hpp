#ifndef PGM_IMAGE_HPP
#define PGM_IMAGE_HPP

#include <fstream>
#include <vector>
#include <errors.hpp>
#include <memory>
#include <histogram.hpp>

class DepthImage;

class PgmImage {
    public:
        PgmImage(std::string filename);
        inline int getWidth() const { return width; }
        inline int getHeight() const { return height; }
        inline const std::vector<int>& getPixels() const { 
            return pixelData;
        }
        inline int getMaxValue() const {
            return maxValue;
        }

        class InvalidDepthError : public std::runtime_error {
        public:
            InvalidDepthError() :
                runtime_error("Pixel depth was something besides 255") {}
        };
    private:
        int width;
        int height;
        int maxValue;
        std::vector<int> pixelData;
    friend class DepthImage;
};

#endif
