// deprecated: no longer using XPM file formats 

// parse XPM and returns a row-major image array

#ifndef XIANGQI_XPMPARSER_HPP
#define XIANGQI_XPMPARSER_HPP

#include "CoreDef.hpp"

// XPM can only store 24-bit image
// pixel channels are stored in RGB order
struct RawImageInfo {
    Integer width{0}, height{0};

    // get the position of R channel of a given pixel
    Integer GetPixelPosition(Integer w, Integer h) const;

    unique_ptr<Byte[]> data;
};

const vector<char> parseXPMSupportedMode{
        'g', 'c'
};

Integer ParseHexString(const string &str);

RawImageInfo ParseXPM(const char **xpm);

#endif //XIANGQI_XPMPARSER_HPP
