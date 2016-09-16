#include <pgm_image.hpp>

// constructor is slightly too large, I think. Oh well.
PgmImage::PgmImage(std::string filename) {
    std::ifstream f(filename);
    // File must start with these two cahracters, so we disallow whitespace
    f >> std::noskipws;
    if(! f.is_open()) {
        throw FileNotFoundError(filename);
    }
    char c1 = '\0', c2 = '\0';
    f >> c1;
    f >> c2;
    if(c1 != 'P' || c2 != '2') {
        throw InvalidFormatError("File lacked proper header");
    }
    // whitespace is now allowed again
    f >> std::skipws;
    if(! (f >> width) || ! (f >> height) || ! (f >> maxValue)) {
        throw InvalidFormatError("Width, Height, or max value not given");
    }
    if(maxValue != 255) {
        throw InvalidDepthError();
    }
    int i;
    while(! f.eof()) {
        f >> i;
        // read put us at EOF
        if(f.eof()) {
            break;
        }
        if(f.fail()) {
            throw InvalidFormatError("Read a non-number!");
        }
        pixelData.push_back(i);
    }
    if(pixelData.size() != width * height) {
        auto pds = std::to_string(pixelData.size());
        auto epds = std::to_string(width * height);
        std::string err = std::string("Expected to have ") \
                          + epds + std::string(" pixels, had ") \
                          + pds;
        throw InvalidFormatError(err);
    }
}
