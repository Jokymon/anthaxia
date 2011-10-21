#include "PsBinLoader.h"
#include "Logging/Logging.h"
#include "plugins/PluginManager.h"
#include "plugins/ImageLoader.h"
#include "plugins/processor/MemoryInterface.h"

#include <iostream>
#include <fstream>

DEFINE_STATIC_LOGGER("loader.Binloader");

PsBinLoader::PsBinLoader()
{ }

PsBinLoader::~PsBinLoader()
{ }

void PsBinLoader::loadImage(std::string fileName, MemoryByteInterface* byteInterface)
{
    LOG_DEBUG("Loading bin file " << fileName);

    std::ifstream file(fileName.c_str(), std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open())
    {
        int size = (int)file.tellg();
        char data;
        for (int i=0; i<size; i++)
        {
            file.read(&data, 1);
            byteInterface->setByte(i, data);
        }
        file.close();

        LOG_DEBUG("Loading complete, informing memory observers");
        byteInterface->transactionComplete();
        LOG_DEBUG("All memory observers are informed");
    }
    else
    {
        LOG_ERROR("Opening bin file " << fileName << " failed");
    }
}

class BinLoaderFactory : public ImageLoaderFactory
{
public:
    virtual ImageLoader* create()
    {
        return new PsBinLoader();
    }
};

STATIC_PLUGIN_INIT void registerBinLoaderImage(PluginManager* pm)
{
    PluginInformationBlock binloaderinfo = {
        "Binary image loader",
        IMAGE_LOADER,
        "A loader for loading plain binary files into memories",
        "Silvan Wegmann",
        "swegmann@narf.ch",
        1, 0, 0,
        100,
        "2.1.2010"
    };
    pm->registerImageLoader("bin", new BinLoaderFactory, binloaderinfo);
}

