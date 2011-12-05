#include "MipsModel.h"
#include "plugins/PluginManager.h"
#include "plugins/processor/MemoryInterface.h"
#include "plugins/processor/MemoryUpdateInterface.h"
#include "Simulator.h"
#include "Poco/Runnable.h"
#include "Poco/Thread.h"
#include <cstdlib>
#include <sstream>
#include <iostream>

using Poco::Runnable;
using Poco::Thread;

class MipsMemoryProxy : public MemoryByteInterface, public MemoryWordInterface {
public:
    MipsMemoryProxy(MipsModel& model)
        : mModel(model)
    { }
    bool getByte(int address, unsigned char& value) const
    {
        return mModel.getSmallMemoryByte(address, value);
    }

    bool setByte(int address, unsigned char value)
    {
        return mModel.setSmallMemoryByte(address, value);
    }

    bool getWord(int address, unsigned int& value) const
    {
        return mModel.getSmallMemoryWord(address, value);
    }

    bool setWord(int address, unsigned int value)
    {
        return mModel.setSmallMemoryWord(address, value);
    }

    void registerMemoryObserver(MemoryObserver* observer)
    {
        mModel.registerMemoryObserver(observer);
    }

    void transactionComplete()
    {
        mModel.transactionComplete();
    }

private:
    MipsModel& mModel;
};

/**
 * Runnable derived class to make the GUI independent of the simulation
 */
class MipsRunner : public Runnable {
public:
    MipsRunner(ProcessorModel* model)
        : mModel(model)
    { }
    void run() {
        mStop = false;
        while (!mStop) {
            mModel->executeInstruction();
        }
    }
    void stop() {
        mStop = true;
    }
private:
    bool mStop;
    ProcessorModel* mModel; // only reference, no ownership taken
};

/**
 * Wrapper class to handle ZThread annoyances of accessing a thread after
 * launching it.
 */
class SimulatorThread {
public:
    SimulatorThread(ProcessorModel* model)
        : mThread(0)
        , mModelRunner(0)
        , mModel(model)
    { }

    ~SimulatorThread()
    {
        this->stop();
    }

    void start()
    {
        mModelRunner = new MipsRunner(mModel);
        mThread = new Thread();
        mThread->start(*mModelRunner);
    }

    void stop()
    {
        if (mModelRunner)
        {
            mModelRunner->stop();
        }
        if (mThread)
        {
            mThread->join();
        }
        // We don't have to delete the mModelRunner, that will be collected by
        // the Thread
        delete mThread;
        mThread = 0;
    }

private:
    Thread* mThread;
    MipsRunner* mModelRunner;
    ProcessorModel* mModel; // only reference, no ownership taken
};

/*-----------------------------------------------------------*/

static std::string _registerNames[34] = {
    "$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
    "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
    "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
    "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra",
    "$hi", "$lo"
};

MipsModel::MipsModel()
: SystemOnChip("MipsModel")
, mSimThread(new SimulatorThread(this))
{
    _reset();
    for (int i=0; i<65536; i++)
    {
        mMemory[i] = 0;
    }
}

MipsModel::~MipsModel()
{
    delete mSimThread;
    mSimThread = 0;
}

void MipsModel::_reset()
{
    mPC          = 0;
    mPCnext      = 4;
    mEPC         = 0;
    mExceptionId = 0;
    mSkip        = false;
    for (int i=0; i<34; i++)
    {
        mRegister[i]          = 0;
        mRegisterDirtyFlag[i] = false;
    }
}

std::string MipsModel::getPluginName()
{
    return "MipsModel";
}

void MipsModel::run()
{
    for (int i=0; i<34; i++)
    {
        mRegisterDirtyFlag[i] = false;
    }
    mSimThread->start();
    SimulationStarted.sigmit();
}

void MipsModel::step()
{
    mSimThread->stop();
    for (int i=0; i<34; i++)
    {
        mRegisterDirtyFlag[i] = false;
    }
    this->executeInstruction();
    SimulationStopped.sigmit();
    this->notifyMemoryChanged();
}

void MipsModel::stop()
{
    mSimThread->stop();
    SimulationStopped.sigmit();
    this->notifyMemoryChanged();
}

void MipsModel::reset()
{
    mSimThread->stop();
    _reset();
    SimulationReset.sigmit();
    this->notifyMemoryChanged();
}

int MipsModel::getPC() const
{
    return mPC;
}

bool MipsModel::getRegister(int index, int& value) const
{
    if (index<0 || index>=34) {
        return false;
    }
    value = mRegister[index];
    return true;
}

bool MipsModel::setRegister(int index, int value)
{
    if (index<0 || index>=34) {
        return false;
    }
    mRegister[index] = value;
    return true;
}

bool MipsModel::getRegisterName(int index, std::string& name) const
{
    if (index<0 || index>=34) {
        return false;
    }
    name = _registerNames[index];
    return true;
}

int MipsModel::getRegisterCount() const
{
    return 34;
}

bool MipsModel::isRegisterDirty(int index) const
{
    if (index<34)
        return mRegisterDirtyFlag[index];
    else
        return false;
}

bool MipsModel::getSmallMemoryByte(int address, unsigned char& value) const
{
    if (address<0 || address>=65536) {
        return false;
    }
    value = mMemory[address];
    return true;
}

bool MipsModel::setSmallMemoryByte(int address, unsigned char value)
{
    if (address<0 || address>=65535) {
        return false;
    }
    mMemory[address] = value;
}

bool MipsModel::getSmallMemoryHalfWord(int address, unsigned short& value) const
{
    if (address<0 || address>=65536-1) {
        return false;
    }
    value = (mMemory[address] << 8) | mMemory[address+1];
    return true;
}

bool MipsModel::setSmallMemoryHalfWord(int address, unsigned short value)
{
    if (address<0 || address>=65535-1) {
        return false;
    }
    mMemory[address] = value >> 8;
    mMemory[address+1] = value & 0xff;
}

bool MipsModel::getSmallMemoryWord(int address, unsigned int& value) const
{
    if (address<0 || address>=65536-3) {
        return false;
    }
    value = (mMemory[address] << 24) | (mMemory[address+1] << 16) |
            (mMemory[address+2] << 8) | mMemory[address+3];
    return true;
}

bool MipsModel::setSmallMemoryWord(int address, unsigned int value)
{
    if (address<0 || address>=65535-3) {
        return false;
    }
    mMemory[address] = value >> 24;
    mMemory[address+1] = (value >> 16) & 0xff;
    mMemory[address+2] = (value >> 8) & 0xff;
    mMemory[address+3] = value & 0xff;
}

void MipsModel::getMemoryList(std::vector<std::string>& list)
{
}

MemoryByteInterface* MipsModel::getByteInterfaceByName(std::string& memory_name)
{
    return new MipsMemoryProxy(*this);
}

MemoryWordInterface* MipsModel::getWordInterfaceByName(std::string& memory_name)
{
    return new MipsMemoryProxy(*this);
}

MemoryByteInterface* MipsModel::getByteInterfaceByIndex(int index)
{
    return new MipsMemoryProxy(*this);
}

MemoryWordInterface* MipsModel::getWordInterfaceByIndex(int index)
{
    return new MipsMemoryProxy(*this);
}

std::string MipsModel::disassembleInstruction(int address) const
{
    std::ostringstream disassembly;
    uint32_t value;
    getSmallMemoryWord(address, value);

    int opcode = (unsigned int)value>>26;
    int s = (value>>21) & 31;
    int t = (value>>16) & 31;
    int d = (value>>11) & 31;
    uint32_t imm16u = uint16_t(value);
    int32_t imm16s = (int32_t)(value&0xffff) | (0xffff0000*((0x8000&value)?1:0));
    int32_t imm16s_shift = imm16s*4-4;  // used for branches
    int shamt = (value >> 6) & 31;
    #define ADDR (imm16s+r[s])

    switch (opcode)
    {
        case 0:
        {
            switch (value & 0x3f)
            {
                case 0x0:
                    if (0==value)
                        disassembly << "nop";
                    else
                        if (0==shamt)
                            disassembly << "move "
                                << _registerNames[d] << ", "
                                << _registerNames[t];
                        else
                            disassembly << "sll "
                                << _registerNames[d] << ", "
                                << _registerNames[t] << ", "
                                << "0x" << std::hex << shamt;
                    break;
                case 0x02:
                    disassembly << "srl "
                        << _registerNames[d] << ", "
                        << _registerNames[t] << ", "
                        << "0x" << std::hex << shamt;
                    break;
                case 0x03:
                    disassembly << "sra "
                        << _registerNames[d] << ", "
                        << _registerNames[t] << ", "
                        << "0x" << std::hex << shamt;
                    break;
                case 0x04:
                    disassembly << "sllv "
                        << _registerNames[d] << ", "
                        << _registerNames[t] << ", "
                        << _registerNames[s];
                    break;
                case 0x06:
                    disassembly << "srlv "
                        << _registerNames[d] << ", "
                        << _registerNames[t] << ", "
                        << _registerNames[s];
                    break;
                case 0x07:
                    disassembly << "srav "
                        << _registerNames[d] << ", "
                        << _registerNames[t] << ", "
                        << _registerNames[s];
                    break;
                case 0x08:
                    disassembly << "jr "
                        << _registerNames[s];
                    break;
                case 0x09:
                    disassembly << "jalr "
                        << _registerNames[s] << ", "
                        << _registerNames[t];
                    break;
                case 0x0c:
                    disassembly << "syscall";
                    break;
                case 0x0d:
                    disassembly << "break";
                    break;
                case 0x10:
                    // TODO: mask out all other registers
                    disassembly << "mfhi "
                        << _registerNames[d];
                    break;
                case 0x11:
                    // TODO: mask out all other registers
                    disassembly << "mthi "
                        << _registerNames[s];
                    break;
                case 0x12:
                    // TODO: mask out all other registers
                    disassembly << "mflo "
                        << _registerNames[d];
                    break;
                case 0x13:
                    // TODO: mask out all other registers
                    disassembly << "mtlo "
                        << _registerNames[s];
                    break;
                case 0x18:
                    disassembly << "mult "
                        << _registerNames[s] << ", "
                        << _registerNames[t];
                    break;
                case 0x19:
                    disassembly << "multu "
                        << _registerNames[s] << ", "
                        << _registerNames[t];
                    break;
                case 0x1a:
                    disassembly << "div "
                        << _registerNames[s] << ", "
                        << _registerNames[t];
                    break;
                case 0x1b:
                    disassembly << "divu "
                        << _registerNames[s] << ", "
                        << _registerNames[t];
                    break;
                case 0x20:
                    disassembly << "add "
                        << _registerNames[d] << ", "
                        << _registerNames[s] << ", "
                        << _registerNames[t];
                    break;
                case 0x21:
                    disassembly << "addu "
                        << _registerNames[d] << ", "
                        << _registerNames[s] << ", "
                        << _registerNames[t];
                    break;
                case 0x22:
                    disassembly << "sub "
                        << _registerNames[d] << ", "
                        << _registerNames[s] << ", "
                        << _registerNames[t];
                    break;
                case 0x23:
                    disassembly << "subu "
                        << _registerNames[d] << ", "
                        << _registerNames[s] << ", "
                        << _registerNames[t];
                    break;
                case 0x24:
                    disassembly << "and "
                        << _registerNames[d] << ", "
                        << _registerNames[s] << ", "
                        << _registerNames[t];
                    break;
                case 0x25:
                    disassembly << "or "
                        << _registerNames[d] << ", "
                        << _registerNames[s] << ", "
                        << _registerNames[t];
                    break;
                case 0x26:
                    disassembly << "xor "
                        << _registerNames[d] << ", "
                        << _registerNames[s] << ", "
                        << _registerNames[t];
                    break;
                case 0x27:
                    disassembly << "nor "
                        << _registerNames[d] << ", "
                        << _registerNames[s] << ", "
                        << _registerNames[t];
                    break;
                case 0x2a:
                    disassembly << "slt "
                        << _registerNames[d] << ", "
                        << _registerNames[s] << ", "
                        << _registerNames[t];
                    break;
                case 0x2b:
                    disassembly << "sltu "
                        << _registerNames[d] << ", "
                        << _registerNames[s] << ", "
                        << _registerNames[t];
                    break;
                default:
                    disassembly << "!!!Unknown instruction!!!";
                    break;
            }
            break;
        }
        case 0x01:
        {
            switch ((value>>16) & 0x1f)
            {
                case 0x0:
                    disassembly << "bltz "
                        << _registerNames[s] << ", "
                        << "0x" << std::hex << address+8+imm16s_shift;
                    break;
                case 0x1:
                    if (s==0)
                    {
                        disassembly << "b "
                            << "0x" << std::hex << address+8+imm16s_shift;
                    } else {
                        disassembly << "bgez "
                            << _registerNames[s] << ", "
                            << "0x" << std::hex << address+8+imm16s_shift;
                    }
                    break;
                case 0x10:
                    disassembly << "bltzal "
                        << _registerNames[s] << ", "
                        << "0x" << std::hex << address+8+imm16s_shift;
                    break;
                case 0x11:
                    if (s==0)
                    {
                        disassembly << "bal "
                            << "0x" << std::hex << address+8+imm16s_shift;
                    } else {
                        disassembly << "bgezal "
                            << _registerNames[s] << ", "
                            << "0x" << std::hex << address+8+imm16s_shift;
                    }
                    break;
            }
            break;
        }
        case 0x02:
            disassembly << "j "
                << "0x" << std::hex << (value & ((1<<25)-1))*4;
            break;
        case 0x03:
            disassembly << "jal "
                << "0x" << std::hex << (value & ((1<<25)-1))*4;
            break;
        case 0x04:
            if ((s==0) && (t==0))
            {
                disassembly << "b "
                    << "0x" << std::hex << address+8+imm16s_shift;
            } else
            {
                disassembly << "beq "
                    << _registerNames[s] << ", "
                    << _registerNames[t] << ", "
                    << "0x" << std::hex << address+8+imm16s_shift;
            }
            break;
        case 0x05:
            if (t==0)
            {
                disassembly << "bnez "
                    << _registerNames[s] << ", "
                    << "0x" << std::hex << address+8+imm16s_shift;
            } else {
                disassembly << "bne "
                    << _registerNames[s] << ", "
                    << _registerNames[t] << ", "
                    << "0x" << std::hex << address+8+imm16s_shift;
            }
            break;
        case 0x06:
            if (t==0)
            {
                disassembly << "blez "
                    << _registerNames[s] << ", "
                    << "0x" << std::hex << address+8+imm16s_shift;
            } else {
                disassembly << "ble "
                    << _registerNames[s] << ", "
                    << _registerNames[t] << ", "
                    << "0x" << std::hex << address+8+imm16s_shift;
            }
            break;
        case 0x07:
            disassembly << "bgt "
                << _registerNames[s] << ", "
                << _registerNames[t] << ", "
                << "0x" << std::hex << address+8+imm16s_shift;
            break;
        case 0x08:
            disassembly << "addi "
                << _registerNames[t] << ", "
                << _registerNames[s] << ", "
                << imm16s;
            break;
        case 0x09:
            if (s==0)
            {
                disassembly << "li "
                    << _registerNames[t] << ", "
                    << imm16s;
            } else
            {
                disassembly << "addiu "
                    << _registerNames[t] << ", "
                    << _registerNames[s] << ", "
                    << imm16s;
            }
            break;
        case 0x0a:
            disassembly << "slti "
                << _registerNames[t] << ", "
                << _registerNames[s] << ", "
                << imm16s;
            break;
        case 0x0b:
            disassembly << "sltiu "
                << _registerNames[t] << ", "
                << _registerNames[s] << ", "
                << imm16s;
            break;
        case 0x0c:
            disassembly << "andi "
                << _registerNames[t] << ", "
                << _registerNames[s] << ", "
                << "0x" << std::hex << imm16u;
            break;
        case 0x0d:
            if (s==0)
            {
                disassembly << "li "
                    << _registerNames[t] << ", "
                    << "0x" << std::hex << imm16u;
            } else
            {
                disassembly << "ori "
                    << _registerNames[t] << ", "
                    << _registerNames[s] << ", "
                    << "0x" << std::hex << imm16u;
            }
            break;
        case 0x0e:
            disassembly << "xori "
                << _registerNames[t] << ", "
                << _registerNames[s] << ", "
                << imm16s;
            break;
        case 0x0f:
            disassembly << "lui "
                << _registerNames[t] << ", "
                << "0x" << std::hex << imm16u;
            break;
        case 0x10:
            disassembly << "mfc0 "
                << _registerNames[t] << ", "
                << "$" << d;
            break;
        case 0x12:
            disassembly << "jr "
                << _registerNames[s];
            break;
        case 0x13:
            disassembly << "jalr "
                << _registerNames[s];
            break;
        case 0x20:
            disassembly << "lb "
                << _registerNames[t] << ", "
                << imm16s << "(" << _registerNames[s] << ")";
            break;
        case 0x21:
            disassembly << "lh "
                << _registerNames[t] << ", "
                << imm16s << "(" << _registerNames[s] << ")";
            break;
        case 0x22:
            disassembly << "lwl "
                << _registerNames[t] << ", "
                << imm16s << "(" << _registerNames[s] << ")";
            break;
        case 0x23:
            disassembly << "lw "
                << _registerNames[t] << ", "
                << imm16s << "(" << _registerNames[s] << ")";
            break;
        case 0x24:
            disassembly << "lbu "
                << _registerNames[t] << ", "
                << imm16s << "(" << _registerNames[s] << ")";
            break;
        case 0x25:
            disassembly << "lhu "
                << _registerNames[t] << ", "
                << imm16s << "(" << _registerNames[s] << ")";
            break;
        case 0x26:
            disassembly << "lwr "
                << _registerNames[t] << ", "
                << imm16s << "(" << _registerNames[s] << ")";
            break;
        case 0x28:
            disassembly << "sb "
                << _registerNames[t] << ", "
                << imm16s << "(" << _registerNames[s] << ")";
            break;
        case 0x29:
            disassembly << "sh "
                << _registerNames[t] << ", "
                << imm16s << "(" << _registerNames[s] << ")";
            break;
        case 0x2a:
            disassembly << "swl "
                << _registerNames[t] << ", "
                << imm16s << "(" << _registerNames[s] << ")";
            break;
        case 0x2b:
            disassembly << "sw "
                << _registerNames[t] << ", "
                << imm16s << "(" << _registerNames[s] << ")";
            break;
        case 0x2e:
            disassembly << "swr "
                << _registerNames[t] << ", "
                << imm16s << "(" << _registerNames[s] << ")";
            break;
        default:
            disassembly << "!!!Unknown instruction!!!";
    }
    return disassembly.str();
}

void MipsModel::executeInstruction()
{
    uint32_t ir;
    getSmallMemoryWord(mPC, ir);
    int *r = mRegister;

    uint32_t opcode = ir>>26;
    int s = (ir>>21) & 31;
    int t = (ir>>16) & 31;
    int d = (ir>>11) & 31;
    uint32_t imm16u = uint16_t(ir);
    int32_t imm16s = (int32_t)(ir&0xffff) | (0xffff0000*((0x8000&ir)?1:0));
    int32_t imm16s_shift = imm16s*4-4;  // used for branches
    int shamt = (ir >> 6) & 31;
    #define ADDR (imm16s+r[s])
    
    r[0] = 0;

    uint32_t epc = mPC + 4;
    if (mPCnext != mPC + 4)
        epc |= 2;
    mPC = mPCnext;
    mPCnext = mPCnext + 4;

    if (mSkip) {
        mSkip = false;
        return;
    }

    uint32_t rSave = r[t];

    // calculate interrupts
    mIrqStatus = 0;
    if (mCounter & (1<<18))
        mIrqStatus |= (1<<3);
    else
        mIrqStatus |= (1<<2);
    bool haveIrq = (mIrqStatus & mIrqMask) != 0; // save result as instructions could change either */

    if (!haveIrq)
    {
        switch(opcode)
        {
            case 0:
                switch(ir & 0x3f)
                {
                    case 0x00:
                        r[d] = r[t] << shamt;
                        if (d!=0)
                            mRegisterDirtyFlag[d] = true; 
                        break;
                    case 0x02:
                        r[d] = r[t] >> shamt;
                        mRegisterDirtyFlag[d] = true; 
                        break;
                    case 0x03:
                        // right shift arithmetic
                        r[d] = int32_t(r[t]) >> shamt; 
                        mRegisterDirtyFlag[d] = true; 
                        break;
                    case 0x04:
                        r[d] = r[t] << r[s];
                        mRegisterDirtyFlag[d] = true; 
                        break;
                    case 0x06:
                        r[d] = r[t] >> r[s];
                        mRegisterDirtyFlag[d] = true; 
                        break;
                    case 0x07:
                        // right shift arithmetic
                        r[d] = int32_t(r[t]) >> r[s];
                        mRegisterDirtyFlag[d] = true; 
                        break;
                    case 0x08: 
                        mPCnext = r[s];
                        break;
                    case 0x09:
                        r[d] = mPCnext;
                        mRegisterDirtyFlag[d] = true; 
                        mPCnext = r[s];
                        break;
                    case 0x0c: // syscall
                        epc |= 1;
                        mExceptionId = 1;
                        break;
                    case 0x0d: // break
                        epc |= 1;
                        mExceptionId = 1;
                        break;
                    case 0x10:
                        r[d] = r[HI];
                        mRegisterDirtyFlag[d] = true; 
                        break;
                    case 0x11:
                        r[HI] = r[s];
                        mRegisterDirtyFlag[HI] = true; 
                        break;
                    case 0x12:
                        r[d] = r[LO];
                        mRegisterDirtyFlag[d] = true; 
                        break;
                    case 0x13:
                        r[LO] = r[s];
                        mRegisterDirtyFlag[LO] = true; 
                        break;
                    case 0x18: // mult
                        {
                            long long val = int32_t(r[s]);
                            val *= int32_t(r[t]);
                            r[HI] = (val >> 32);
                            r[LO] = (val & 0xffffffff);
                            mRegisterDirtyFlag[LO] = true; 
                            mRegisterDirtyFlag[HI] = true; 
                            break;
                        }
                    case 0x19: // multu
                        {
                            unsigned long long val = r[s];
                            val *= r[t];
                            r[HI] = (val >> 32);
                            r[LO] = (val & 0xffffffff);
                            mRegisterDirtyFlag[LO] = true; 
                            mRegisterDirtyFlag[HI] = true; 
                            break;
                        }
                    case 0x1A: // div
                        {
                            if (r[t] == 0)
                            {
                                // TODO: handle div by zero
                                break;
                            }
                            r[LO] = int32_t(r[s]) / int32_t(r[t]);
                            r[HI] = int32_t(r[s]) % int32_t(r[t]);
                            mRegisterDirtyFlag[LO] = true; 
                            mRegisterDirtyFlag[HI] = true; 
                            break;
                        }
                    case 0x1B: // divu
                        {
                            if (r[t] == 0)
                            {
                                // TODO: handle div by zero
                                break;
                            }
                            r[LO] = r[s] / r[t];
                            r[HI] = r[s] % r[t];
                            mRegisterDirtyFlag[LO] = true; 
                            mRegisterDirtyFlag[HI] = true; 
                            break;
                        }
                    case 0x20:
                        r[d] = r[s] + r[t];
                        mRegisterDirtyFlag[d] = true; 
                        break;
                    case 0x21:
                        r[d] = r[s] + r[t];
                        mRegisterDirtyFlag[d] = true; 
                        break;
                    case 0x22:
                        r[d] = r[s] - r[t];
                        mRegisterDirtyFlag[d] = true; 
                        break;
                    case 0x23:
                        r[d] = r[s] - r[t];
                        mRegisterDirtyFlag[d] = true; 
                        break;
                    case 0x24:
                        r[d] = r[s] & r[t];
                        mRegisterDirtyFlag[d] = true; 
                        break;
                    case 0x25:
                        r[d] = r[s] | r[t];
                        mRegisterDirtyFlag[d] = true; 
                        break;
                    case 0x26:
                        r[d] = r[s] ^ r[t];
                        mRegisterDirtyFlag[d] = true; 
                        break;
                    case 0x27:
                        r[d] = ~(r[s] | r[t]);
                        mRegisterDirtyFlag[d] = true; 
                        break;
                    case 0x2A:
                        r[d] = (int32_t(r[s]) < int32_t(r[t]));
                        mRegisterDirtyFlag[d] = true; 
                        break;
                    case 0x2B:
                        r[d] = (r[s] < r[t]);
                        mRegisterDirtyFlag[d] = true; 
                        break;
                }
                break;
            case 1:
                switch((ir >> 16) & 31)
                {
                    case 0x00:
                        if (int32_t(r[s]) < int32_t(0)) mPCnext += imm16s_shift;
                        break;
                    case 0x01:
                        if (int32_t(r[s]) >= int32_t(0)) mPCnext += imm16s_shift;
                        break;
                    case 0x10:
                        if (int32_t(r[s]) < int32_t(0))
                        {
                            mRegister[31] = mPCnext;
                            mRegisterDirtyFlag[31] = true; 
                            mPCnext += imm16s_shift;
                        }
                        break;
                    case 0x11:
                        if (int32_t(r[s]) >= int32_t(0))
                        {
                            mRegister[31] = mPCnext;
                            mRegisterDirtyFlag[31] = true; 
                            mPCnext += imm16s_shift;
                        }
                        break;
                }
                break;
            case IST_J:
                mPCnext = (ir & ((1<<25)-1))*4;
                break;
            case IST_JAL:
                mRegister[31] = mPCnext;
                mRegisterDirtyFlag[31] = true; 
                mPCnext = (ir & ((1<<25)-1))*4;
                break;
            case IST_BEQ:
                if (r[s] == r[t]) mPCnext += imm16s_shift;
                break;
            case IST_BNE:
                if (r[s] != r[t]) mPCnext += imm16s_shift;
                break;
            case IST_BLE:
                if (int32_t(r[s]) <= int32_t(r[t])) mPCnext += imm16s_shift;
                break;
            case IST_BGT:
                if (int32_t(r[s]) > int32_t(r[t])) mPCnext += imm16s_shift;
                break;
            case IST_ADDI:
                r[t] = r[s] + imm16s;
                mRegisterDirtyFlag[t] = true; 
                break;
            case IST_ADDIU:
                r[t] = r[s] + imm16s;
                mRegisterDirtyFlag[t] = true; 
                break;
            case IST_SLTI:
                r[t] = (int32_t(r[s]) < imm16s);
                mRegisterDirtyFlag[t] = true; 
                break;
            case IST_SLTIU:
                r[t] = (r[s] < imm16u);
                mRegisterDirtyFlag[t] = true; 
                break;
            case IST_ANDI:
                r[t] = r[s] & imm16u;
                mRegisterDirtyFlag[t] = true; 
                break;
            case IST_ORI:
                r[t] = r[s] | imm16u;
                mRegisterDirtyFlag[t] = true; 
                break;
            case IST_XORI:
                r[t] = r[s] ^ imm16u;
                mRegisterDirtyFlag[t] = true; 
                break;
            case IST_LUI:
                r[t] = imm16u << 16;
                mRegisterDirtyFlag[t] = true; 
                break;
            case IST_MxC0:
                if ((ir & (1<<23)) == 0) // MFC0
                {
                    if (d == 12)
                        r[t] = mStatus;
                    else if (d == 14)
                        r[t] = mEPC;
                    else
                        r[t] = 0x0;
                } 
                else // MTC0
                {
                    if (d == 12)
                        mStatus = r[t];
                }
                break;
            case IST_LB:
                unsigned char byte;
                getSmallMemoryByte(ADDR, byte);
                r[t] = byte;
                if (r[t] & 0x80)
                {
                    r[t] = 0xffffff00 | r[t];
                    mRegisterDirtyFlag[t] = true; 
                }
                break;
            case IST_LH:
                unsigned short halfword;
                getSmallMemoryHalfWord(ADDR, halfword);
                r[t] = halfword;
                if (r[t] & 0x8000)
                {
                    r[t] = 0xffff0000 | r[t];
                    mRegisterDirtyFlag[t] = true; 
                }
                break;
            case IST_LW:
                unsigned int word;
                getSmallMemoryWord(ADDR, word);
                r[t] = word;
                mRegisterDirtyFlag[t] = true; 
                break;
            case IST_LBU:
                getSmallMemoryByte(ADDR, byte);
                r[t] = byte;
                mRegisterDirtyFlag[t] = true; 
                break;
            case IST_SB:
                setSmallMemoryByte(ADDR, r[t] & 0xff);
                break;
            case IST_SH:
                setSmallMemoryHalfWord(ADDR, r[t] & 0xffff);
                break;
            case IST_SW:
                setSmallMemoryWord(ADDR, r[t]);
                break;
            default:
                // Send a notice?
                break;
        }
    }

    mPCnext &= ~3; // bits 0 and 1 can be set because of EPC

    if (mExceptionId || haveIrq)
    {
        r[t] = rSave;
        mEPC = epc;
        mPCnext = 0x3c;
        mSkip = 1;
        mExceptionId = 0;
    }

    mCounter += 1;
}

void MipsModel::registerMemoryObserver(MemoryObserver* observer)
{
    mMemoryObservers.push_back(observer);
}

void MipsModel::notifyMemoryChanged()
{
    for (std::vector<MemoryObserver*>::iterator iter = mMemoryObservers.begin();
         iter != mMemoryObservers.end();
         iter++)
    {
        (*iter)->handleMemoryChanged();
    }
}

void MipsModel::transactionComplete()
{
    this->notifyMemoryChanged();
}

class MipsModelFactory : public ProcessorControlFactory
{
public:
    virtual ProcessorControl* create()
    {
        return new MipsModel();
    }
};

PLUGIN_API void registerPlugin(PluginManager* pm)
{
    PluginInformationBlock mipsmodelinfo = {
        "MIPS Plasma simulation model",
        PROCESSOR_MODEL,
        "A simple simulation model of the MIPS compatible Plasma processor core",
        "Silvan Wegmann",
        "swegmann@narf.ch",
        1, 0, 0,
        100,
        "13.2.2009"
    };
    pm->registerProcessorControl("MipsModel", new MipsModelFactory, mipsmodelinfo);
}
