/***********************************************************************
 *
 * This file is part of Anthaxia.
 * 
 * Anthaxia is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Anthaxia is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Anthaxia.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

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

