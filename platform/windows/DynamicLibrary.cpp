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

