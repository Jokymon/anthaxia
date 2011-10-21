#ifndef _PSBINLOADER_H
#define _PSBINLOADER_H

#include "plugins/ImageLoader.h"
#include <string>

class MemoryByteInterface;

class PsBinLoader : public ImageLoader
{
public:
    PsBinLoader();
    ~PsBinLoader();

    void loadImage(std::string fileName, MemoryByteInterface* byteInterface);
};

#endif
