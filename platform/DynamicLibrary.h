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

#ifndef _DYNAMICLIBRARY_H
#define _DYNAMICLIBRARY_H

#include <string>

class DynamicLibrary
{
public:
    static DynamicLibrary* load(const std::string& path);
    ~DynamicLibrary();
    void* getSymbol(const std::string& name);

    static const std::string DL_PREFIX;
    static const std::string DL_SUFFIX;
private:
    DynamicLibrary();
    DynamicLibrary(void* handle);
    DynamicLibrary(const DynamicLibrary&);
private:
    void* handle_;
};

#endif
