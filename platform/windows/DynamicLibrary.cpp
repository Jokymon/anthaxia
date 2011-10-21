#include "../DynamicLibrary.h"
#include "Logging/Logging.h"
#include <windows.h>

DEFINE_STATIC_LOGGER("platform.windows.DynamicLibrary");

const std::string DynamicLibrary::DL_PREFIX = "";
const std::string DynamicLibrary::DL_SUFFIX = "dll";

DynamicLibrary* DynamicLibrary::load(const std::string& path)
{
    HINSTANCE hdll = LoadLibraryA(path.c_str());
    return new DynamicLibrary(hdll);
}

DynamicLibrary::DynamicLibrary(void* handle)
: handle_(handle)
{ }

DynamicLibrary::~DynamicLibrary()
{
}

void* DynamicLibrary::getSymbol(const std::string& name)
{
    LOG_DEBUG("getting symbol: " << name.c_str());
    void* symbol = (void*)GetProcAddress((HINSTANCE)handle_, name.c_str());
    char* error = NULL;
    if (NULL==symbol) {
        DWORD dwError = GetLastError();
        FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM, 
                       NULL, 
                       dwError, 
                       0, 
                       (LPTSTR)&error, 
                       0, 
                       NULL);
        LOG_ERROR("Couldn't find " << name << ": " << error);
        LocalFree(error);
    }

    return symbol;
}

