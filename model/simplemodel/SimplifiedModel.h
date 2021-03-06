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

#ifndef _SIMPLIFIEDMODEL_H
#define _SIMPLIFIEDMODEL_H

#include "Simulator.h"
#include "plugins/Plugin.h"
#include "plugins/processor/ProcessorControl.h"
#include <string>
#include <vector>
#ifdef _MSC_VER
#include "platform/windows/stdint.h"
#else
#include <stdint.h>
#endif

class SimulatorThread;
class SimulationObserver;

class SimplifiedModel : public ProcessorControl, public ProcessorModel
{
public:
    SimplifiedModel();
    virtual ~SimplifiedModel();
    std::string getPluginName();
    ///////////////////////////////////////
    // Simulation control interface
    ///////////////////////////////////////
    /**
     * Start the simulation in a separate thread
     */
    void run();
    /**
     * Execute one instruction of this processor model. When the simulation was
     * previously running it will be stopped.
     */
    void step();
    /**
     * Stop the currently running simulation
     */
    void stop();
    /**
     * Reset the processor
     */
    void reset();

    ////////////////////////////////////////////
    // Simulation information interface
    ////////////////////////////////////////////
    /**
     * Returns the current value of the program counter.
     */
    int getPC() const;
    /**
     * Get the content of the processor register with the given index. When the
     * given index is available on this processor true is returned and the
     * value is passed back by reference.
     */
    bool getRegister(int index, int& value) const;
    /**
     * Set the content of the processor register with the given index. When the
     * given index is available on this processor true is returned and the
     * value is set.
     */
    bool setRegister(int index, int value);
    /**
     * Returns the name for the register with the given index via the name
     * reference variable if the return value is true. When false is returned,
     * the given index is not a valid register.
     */
    bool getRegisterName(int index, std::string& name) const;
    /**
     * Returns the number of valid registers for this processor. Registers with
     * an index in the range 0<=index<getRegisterCount() are valid processor
     * registers.
     */
    int getRegisterCount() const;
    /**
     * Indicates whether the register with the given index has been changed
     * since the stop of the simulation.
     */
    bool isRegisterDirty(int index) const;

    ///////////////////////////////////
    // Processor memory interface
    ///////////////////////////////////
    ///////////////////////////////////
    bool getSmallMemoryByte(int address, unsigned char& value) const;
    bool setSmallMemoryByte(int address, unsigned char value);
    bool getSmallMemoryHalfWord(int address, unsigned short& value) const;
    bool setSmallMemoryHalfWord(int address, unsigned short value);
    bool getSmallMemoryWord(int address, unsigned int& value) const;
    bool setSmallMemoryWord(int address, unsigned int value);

    // Multi bus memory interface
    void getMemoryList(std::vector<std::string>& list);
    MemoryByteInterface* getByteInterfaceByName(std::string& memory_name);
    MemoryWordInterface* getWordInterfaceByName(std::string& memory_name);
    MemoryByteInterface* getByteInterfaceByIndex(int index);
    MemoryWordInterface* getWordInterfaceByIndex(int index);

    ///////////////////////////////
    // Disassembler interface
    ///////////////////////////////
    std::string disassembleInstruction(int address) const;

    ///////////////////////////////////////
    // Generic processor model interface
    ///////////////////////////////////////
    virtual void executeInstruction();

    //////////////////////////////
    // Notification interface
    //////////////////////////////

private:
    void _reset();

    int mPC;
    int mRegister[32];
    bool mRegisterDirtyFlag[32];
    unsigned char mMemory[65536];
    SimulatorThread* mSimThread;

    friend class SimplifiedModelMemoryProxy;
};

class PluginManager;
PLUGIN_API void registerPlugin(PluginManager* pm);

#endif
