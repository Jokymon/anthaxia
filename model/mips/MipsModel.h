#ifndef _MIPSMODEL_H
#define _MIPSMODEL_H

#include "Simulator.h"
#include "model/SystemOnChip.h"
#include "plugins/Plugin.h"
#include "plugins/processor/ProcessorControl.h"
#include <string>
#include <vector>
#ifdef _MSC_VER
#include "platform/windows/stdint.h"
#endif

class MemoryObserver;
class SimulatorThread;
class SimulationObserver;

class MipsModel : public ProcessorControl, public ProcessorModel, public SystemOnChip
{
public:
    MipsModel();
    virtual ~MipsModel();
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

    void registerMemoryObserver(MemoryObserver* observer);
    void transactionComplete();

    ////////////////////////////////////
    // Simulation model tree functions
    ////////////////////////////////////
    virtual int parentIndex();
    virtual int childCount();
    virtual SimulationObject* getChild(int index);

private:
    enum {
        IST_J      = 2,
        IST_JAL    = 3,
        IST_BEQ    = 4,
        IST_BNE    = 5,
        IST_BLE    = 6,
        IST_BGT    = 7,
        IST_ADDI   = 8,
        IST_ADDIU  = 9,
        IST_SLTI   = 10,
        IST_SLTIU  = 11,
        IST_ANDI   = 12,
        IST_ORI    = 13,
        IST_XORI   = 14,
        IST_LUI    = 15,
        IST_MxC0   = 16,
        IST_LB     = 32,
        IST_LH     = 33,
        IST_LW     = 35,
        IST_LBU    = 37,
        IST_SB     = 40,
        IST_SH     = 41,
        IST_SW     = 43
    };

    enum {
        HI = 32,
        LO = 33
    };

  private:
    void notifyMemoryChanged();
    std::vector<MemoryObserver*> mMemoryObservers;

    void _reset();

    int mPC, mPCnext, mEPC;
    int mExceptionId;
    bool mSkip;
    uint32_t mStatus;
    int mRegister[34];
    bool mRegisterDirtyFlag[34];

    uint32_t mCounter;

    uint32_t mIrqMask;
    uint32_t mIrqStatus;

    unsigned char mMemory[65536];
    SimulatorThread* mSimThread;

    friend class MipsMemoryProxy;
};

class PluginManager;
PLUGIN_API void registerPlugin(PluginManager* pm);

#endif
