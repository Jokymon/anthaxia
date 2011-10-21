#ifndef _IMAGELOADER_H
#define _IMAGELOADER_H

#include "plugins/Plugin.h"

class MemoryByteInterface;

class ImageLoader
{
public:
    virtual void loadImage(std::string fileName, MemoryByteInterface* byteInterface) =0;
};

typedef PluginFactory<ImageLoader> ImageLoaderFactory;

#endif
