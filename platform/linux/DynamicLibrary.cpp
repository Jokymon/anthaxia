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

