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

#ifndef _PROCESSORCONTROL_H
#define _PROCESSORCONTROL_H

#include "sigslot/sigslot.h"
#include "plugins/Plugin.h"
#include <string>
#include <vector>

class MemoryByteInterface;
class MemoryWordInterface;
class SimulationObserver;

class ProcessorControl
{
public:
    ///////////////////////////////////////
    // Simulation control interface
    ///////////////////////////////////////
    /**
     * Start the simulation in a separate thread
     */
    virtual void run() =0;
    /**
     * Execute one instruction of this processor model. When the simulation was
     * previously running it will be stopped.
     */
    virtual void step() =0;
    /**
     * Stop the currently running simulation
     */
    virtual void stop() =0;
    /**
     * Reset the processor
     */
    virtual void reset() =0;

    ////////////////////////////////////////////
    // Simulation information interface
    ////////////////////////////////////////////
    /**
     * Returns the current value of the program counter.
     */
    virtual int getPC() const =0;
    /**
     * Get the content of the processor register with the given index. When the
     * given index is available on this processor true is returned and the
     * value is passed back by reference.
     */
    virtual bool getRegister(int index, int& value) const =0;
    /**
     * Set the content of the processor register with the given index. When the
     * given index is available on this processor true is returned and the
     * value is set.
     */
    virtual bool setRegister(int index, int value) =0;
    /**
     * Returns the name for the register with the given index via the name
     * reference variable if the return value is true. When false is returned,
     * the given index is not a valid register.
     */
    virtual bool getRegisterName(int index, std::string& name) const =0;
    /**
     * Returns the number of valid registers for this processor. Registers with
     * an index in the range 0<=index<getRegisterCount() are valid processor
     * registers.
     */
    virtual int getRegisterCount() const =0;
    /**
     * Indicates whether the register with the given index has been changed
     * since the stop of the simulation.
     */
    virtual bool isRegisterDirty(int index) const =0;

    ///////////////////////////////////
    // Processor memory interface
    ///////////////////////////////////
    virtual bool getSmallMemoryByte(int address, unsigned char& value) const =0;
    virtual bool setSmallMemoryByte(int address, unsigned char value) =0;
    virtual bool getSmallMemoryHalfWord(int address, unsigned short& value) const =0;
    virtual bool setSmallMemoryHalfWord(int address, unsigned short value) =0;
    virtual bool getSmallMemoryWord(int address, unsigned int& value) const =0;
    virtual bool setSmallMemoryWord(int address, unsigned int value) =0;

    // Multi bus memory interface
    virtual void getMemoryList(std::vector<std::string>& list) =0;
    virtual MemoryByteInterface* getByteInterfaceByName(std::string& memory_name) =0;
    virtual MemoryWordInterface* getWordInterfaceByName(std::string& memory_name) =0;
    virtual MemoryByteInterface* getByteInterfaceByIndex(int index) =0;
    virtual MemoryWordInterface* getWordInterfaceByIndex(int index) =0;

    ///////////////////////////////
    // Disassembler interface
    ///////////////////////////////
    virtual std::string disassembleInstruction(int value) const =0;

    //////////////////////////////
    // Notification interface
    //////////////////////////////
    sigslot::signal0<> SimulationStarted;
    sigslot::signal0<> SimulationStopped;
    sigslot::signal0<> SimulationReset;
};

typedef PluginFactory<ProcessorControl> ProcessorControlFactory;

#endif
