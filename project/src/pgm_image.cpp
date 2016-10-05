#include <pgm_image.hpp>

static void depthError(int in) {
    std::stringstream s;
    s << "Invalid depth, expected 255, got " << in;
    throw InvalidFormatError(s.str());
}

// constructor is slightly too large, I think. Oh well.
PgmImage::PgmImage(std::string filename) {
    std::ifstream f(filename);
    f >> std::noskipws;
    // File must start with these two cahracters, so we disallow whitespace
    if(! f.is_open()) {
        throw FileNotFoundError(filename);
    }
    char c1 = '\0', c2 = '\0';
    f >> c1;
    f >> c2;
    if(c1 != 'P' || c2 != '2') {
        throw InvalidFormatError("File lacked proper header");
    }
    f >> std::skipws;
    // whitespace is now allowed again
    if(! (f >> width) || ! (f >> height) || ! (f >> maxValue)) {
        throw InvalidFormatError("Width, Height, or max value not given");
    }
    if(maxValue != 255) {
        depthError(maxValue);
    }
    if(height < 0 || width < 0) {
        throw InvalidFormatError("Height or width impossible");
    }
    int i;
    while(! f.eof() && f >> i) {
        if(f.fail()) {
            throw InvalidFormatError("Read a non-number! (1)");
        }
        if(i < 0 || i > 255) {
            throw BadNumberError(i);
        }
        pixelData.push_back(i);
    }
    if(! f.eof()) {
        throw InvalidFormatError("Read a non-number!");
    }
    if(pixelData.size() != (unsigned int) height * width) {
        throw InvalidFormatError("File's list of ints is too big or small");
    }
}
