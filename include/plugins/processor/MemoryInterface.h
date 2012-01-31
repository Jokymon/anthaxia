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

#ifndef _MEMORYINTERFACE_H
#define _MEMORYINTERFACE_H

class MemoryObserver;

class MemoryByteInterface {
public:
    virtual bool getByte(int address, unsigned char& value) const =0;
    virtual bool setByte(int address, unsigned char value) =0;
    virtual void registerMemoryObserver(MemoryObserver* observer) =0;
    /// call this function to indicate that one or multiple memory changes via
    /// setByte have been completed and that registered memory observers should be
    /// notified about it
    virtual void transactionComplete() =0;
};

class MemoryWordInterface {
public:
    virtual bool getWord(int address, unsigned int& value) const =0;
    virtual bool setWord(int address, unsigned int value) =0;
    virtual void registerMemoryObserver(MemoryObserver* observer) =0;
};

#endif
