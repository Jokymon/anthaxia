#include "../DynamicLibrary.h"
#include "Logging/Logging.h"
#include <dlfcn.h>

DEFINE_STATIC_LOGGER("platform.linux.DynamicLibrary");

const std::string DynamicLibrary::DL_PREFIX = "lib";
const std::string DynamicLibrary::DL_SUFFIX = "so";

DynamicLibrary* DynamicLibrary::load(const std::string& path)
{
    void* hdll = dlopen(path.c_str(), RTLD_NOW | RTLD_GLOBAL);
    return new DynamicLibrary(hdll);
}

DynamicLibrary::DynamicLibrary(void* handle)
: handle_(handle)
{ }

DynamicLibrary::~DynamicLibrary()
{
    dlclose(handle_);
}

void* DynamicLibrary::getSymbol(const std::string& name)
{
    LOG_DEBUG("Getting symbol: " << name.c_str());
    dlerror();
    void* symbol = dlsym(handle_, name.c_str());
    const char *error;
    if ((error = dlerror())) {
        LOG_ERROR("Couldn't find " << name << ": " << error);
    }
    return symbol;
}

