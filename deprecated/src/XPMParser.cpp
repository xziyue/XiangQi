#include "XPMParser.hpp"

Integer RawImageInfo::GetPixelPosition(Integer w, Integer h) const {
    return 3 * (h * this->width + w);
}

Integer ParseHexString(const string &str) {
    vector<Integer> digits;
    digits.reserve(str.size());
    for (auto item : str) {
        if (item >= '0' && item <= '9') {
            digits.push_back(item - '0');
            continue;
        }

        if (item >= 'a' && item <= 'f') {
            item -= 'a' - 'A';
        }

        if (item >= 'A' && item <= 'F') {
            digits.push_back(item - 'A' + 10);
            continue;
        }

        throw runtime_error{string{"invalid Hex Digit: "} + string{item}};
    }

    Integer result = 0;
    Integer base = 1;
    for (auto iter = digits.rbegin(); iter != digits.rend(); ++iter) {
        result += base * (*iter);
        base *= 16;
    }

    return result;
};

RawImageInfo ParseXPM(const char **xpm) {
    Integer width{0}, height{0}, numOfColor{0}, numOfCharPerColor{0};
    stringstream sstr;
    sstr << xpm[0];
    sstr >> width >> height >> numOfColor >> numOfCharPerColor;

    AlwaysAssert(width > 0, "invalid XPM width");
    AlwaysAssert(height > 0, "invalid XPM height");
    AlwaysAssert(numOfColor > 0, "invalid XPM number of color");
    AlwaysAssert(numOfCharPerColor > 0, "invalid XPM number of character per color");

    Integer arrayLength = 3 * width * height;

    sstr.str("");
    sstr.clear();

    map<string, array<Byte, 3>> colorMap;

    string colorKey{};
    colorKey.resize(numOfCharPerColor);
    string colorString{};
    colorString.resize(6);
    // read the colors, create mapping
    for (Integer i = 1; i < 1 + numOfColor; ++i) {

        Integer j = 0;

        for (; j < numOfCharPerColor; ++j) {
            colorKey[j] = xpm[i][j];
        }

        // skip the delimiter
        ++j;
        // check the mode string
        auto modeFindIter = find(parseXPMSupportedMode.begin(), parseXPMSupportedMode.end(), xpm[i][j]);
        if (modeFindIter == parseXPMSupportedMode.end()) {
            throw runtime_error{string{"unsupported XPM color mode: "} + string{xpm[i][j]}};
        }

        // skip the delimiter and the hash sign
        j += 3;
        // extract the color string
        // the length of the color string should be exactly 6

        for (Integer k = 0; k < 6; ++k) {
            colorString[k] = xpm[i][j + k];
        }

        array<Byte, 3> myColor{};
        for (Integer k = 0; k < 3; ++k) {
            myColor[k] = (Byte) ParseHexString(colorString.substr(2 * k, 2));
        }

        colorMap.insert(make_pair(colorKey, move(myColor)));
    }


    // create buffer
    unique_ptr<Byte[]> buffer{new Byte[arrayLength]};


    Integer dataPointer = 0;
    for (Integer i = 1 + numOfColor; i < 1 + numOfColor + height; ++i) {
        Integer colPointer = 0;
        for (Integer j = 0; j < width; ++j) {
            // read character from array
            for (Integer k = 0; k < numOfCharPerColor; ++k) {
                colorKey[k] = xpm[i][colPointer++];
            }
            // get corresponding color
            const auto &color = colorMap.at(colorKey);
            // fill the color into the buffer
            for (Integer k = 0; k < 3; ++k) {
                buffer[dataPointer++] = color[k];
            }
        }
    }

    RawImageInfo imgInfo;
    imgInfo.width = width;
    imgInfo.height = height;
    imgInfo.data = move(buffer);

    return imgInfo;

}