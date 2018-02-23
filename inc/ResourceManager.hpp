

#ifndef XIANGQI_RESOURCEMANAGER_HPP
#define XIANGQI_RESOURCEMANAGER_HPP

#include "CoreDef.hpp"

const string resourceFilenamePrefix{R"(..\res\)"};

extern bool resourceInitialized;

string GetFullFilename(const string &filename);

void InitializeResource();

string GetTextResource(const string &filename);

// guaranteed to output a RGB image
CImgType GetImageResource(const string &filename);

unique_ptr<Byte[]> GetInverleavedImageArray(const CImgType &img);

#endif //XIANGQI_RESOURCEMANAGER_HPP
