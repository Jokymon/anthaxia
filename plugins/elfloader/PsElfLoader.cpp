#include "PsElfLoader.h"
#include "ELFIO.h"
#include "Logging/Logging.h"
#include "plugins/PluginManager.h"
#include "plugins/ImageLoader.h"
#include "plugins/processor/MemoryInterface.h"

#include <iostream>

DEFINE_STATIC_LOGGER("loader.Elfloader");

PsElfLoader::PsElfLoader()
{ }

PsElfLoader::~PsElfLoader()
{ }

void PsElfLoader::loadImage(std::string fileName, MemoryByteInterface* byteInterface)
{
    LOG_DEBUG("Loading elf file " << fileName);
    IELFI* pReader;
    ELFIO::GetInstance()->CreateELFI( &pReader );
    pReader->Load(fileName);

    int nSecNo = pReader->GetSectionsNum();
    for (int i=0; i<nSecNo; ++i)
    {
        const IELFISection* pSec = pReader->GetSection( i );

        if (pSec->GetName()==".text")
        {
            const char* image = pSec->GetData();
            for (int i=0; i<pSec->GetSize(); i++)
            {
                byteInterface->setByte(i, image[i]);
            }
        }

        pSec->Release();
    }

    pReader->Release();
    LOG_DEBUG("Loading complete, informing memory observers");
    byteInterface->transactionComplete();
    LOG_DEBUG("All memory observers are informed");
}

class ElfLoaderFactory : public ImageLoaderFactory
{
public:
    virtual ImageLoader* create()
    {
        return new PsElfLoader();
    }
};

STATIC_PLUGIN_INIT void registerElfLoaderImage(PluginManager* pm)
{
    PluginInformationBlock elfloaderinfo = {
        "Elf image loader",
        IMAGE_LOADER,
        "A loader for loading executable sections from elf files into memories",
        "Silvan Wegmann",
        "swegmann@narf.ch",
        1, 0, 0,
        100,
        "13.2.2009"
    };
    pm->registerImageLoader("elf", new ElfLoaderFactory, elfloaderinfo);
}

