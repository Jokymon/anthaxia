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
