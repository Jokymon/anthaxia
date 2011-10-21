#ifndef _PSELFLOADER_H
#define _PSELFLOADER_H

#include "plugins/ImageLoader.h"
#include <string>

class MemoryByteInterface;

class PsElfLoader : public ImageLoader
{
public:
    PsElfLoader();
    ~PsElfLoader();

    void loadImage(std::string fileName, MemoryByteInterface* byteInterface);
};

#endif
