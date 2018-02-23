#include "ResourceManager.hpp"

bool resourceInitialized = false;

string GetFullFilename(const string &filename){
    return resourceFilenamePrefix + filename;
}

void InitializeResource(){
    if(!resourceInitialized){
        // test whether the folder is correct
        fstream infile;
        infile.open(GetFullFilename("RES.TEST"), ios::in|ios::binary);
        if(!infile.good()){
            throw runtime_error{"failed to find resource, please make sure resourceFilenamePrefix is correct, and that the resource "
                                        "folder is valid."};
        }
        resourceInitialized = true;
    }
}

string GetTextResource(const string &filename){
    AlwaysAssert(resourceInitialized, "resource is not initialized");
    auto fullName = move(GetFullFilename(filename));

    fstream infile{fullName, ios::in|ios::binary};
    if(!infile.good()){
        throw runtime_error{string{"failed to open file "} + fullName};
    }

    infile.seekg(0, ios::end);
    Integer fileLength = (Integer)infile.tellg();
    infile.seekg(0, ios::beg);

    unique_ptr<char[]> buffer{new char[fileLength + 1]};
    buffer[fileLength] = '\0';

    infile.read(buffer.get(), fileLength);

    stringstream sstr;
    sstr << buffer.get();
    return sstr.str();
}

CImgType GetImageResource(const string &filename){
    static_assert(is_same<typename CImgType::value_type, Byte>::value, "unsupported CImgType value type");

    AlwaysAssert(resourceInitialized, "resource is not initialized");
    auto fullName = move(GetFullFilename(filename));
    CImgType result;
    result.load(fullName.c_str());

    AlwaysAssert(result.depth() == 1, "invalid image format: not 2D image");

    if(result.spectrum() != 4){
        CImgType newResult(result.width(), result.height(), 1, 4);

        if(result.spectrum() == 1){ // grayscale
            cimg_forXY(newResult, x, y)
            {
                for (Integer i = 0; i < 3; ++i) {
                    newResult(x, y, 0, i) = result(x, y, 0, 0);
                }
                // this only works when value_type is Byte
                newResult(x, y, 0, 3) = numeric_limits<typename CImgType::value_type>::max();
            }
        }
        else if(result.spectrum() == 2){ // grayscale with transparency
            cimg_forXY(newResult, x, y)
            {
                for (Integer i = 0; i < 3; ++i) {
                    newResult(x, y, 0, i) = result(x, y, 0, 0);
                }
                newResult(x, y, 0, 3) = result(x, y, 0, 1);
                //newResult(x, y, 0, 3) = numeric_limits<typename CImgType::value_type>::max();
            }
        }
        else if(result.spectrum() == 3){ // RGB
            cimg_forXY(newResult, x, y)
            {
                for (Integer i = 0; i < 3; ++i) {
                    newResult(x, y, 0, i) = result(x, y, 0, i);
                }
                newResult(x, y, 0, 3) = numeric_limits<typename CImgType::value_type>::max();
            }
        }
        else{
            throw runtime_error{"unsupported image spectrum number"};
        }
        return newResult;
    } else{
        return result;
    }

}

unique_ptr<Byte[]> GetInverleavedImageArray(const CImgType &img){
    AlwaysAssert(img.depth() == 1 && img.spectrum() == 4, "image format mismatch");
    Integer length = img.width() * img.height() * img.spectrum();

    unique_ptr<Byte[]> result{new Byte[length]};
    Integer pointer = 0;
    cimg_forXY(img, x, y){
        for(Integer i = 0; i < (Integer)img.spectrum(); ++i){
            result[pointer++] = img(x, y, 0, i);
        }
    }

    return result;
}