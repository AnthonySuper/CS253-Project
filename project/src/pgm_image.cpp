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
    while(true) {
        f >> i;
        // read put us at EOF
        if(f.eof() && f.fail()) {
            break;
        }
        else if(f.fail()) {
            throw InvalidFormatError("Read a non-number!");
        }
        pixelData.push_back(i);
    }
}
