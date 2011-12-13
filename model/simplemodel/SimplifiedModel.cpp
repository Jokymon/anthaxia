#include "SimplifiedModel.h"
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

class SimplifiedModelMemoryProxy : public MemoryByteInterface, public MemoryWordInterface {
public:
    SimplifiedModelMemoryProxy(SimplifiedModel& model)
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
        mObservers.push_back(observer);
    }

    void transactionComplete()
    {
        this->notifyMemoryChanged();
    }

private:
    void notifyMemoryChanged()
    {
        for (std::vector<MemoryObserver*>::iterator iter = mObservers.begin();
             iter != mObservers.end();
             iter++)
        {
            (*iter)->handleMemoryChanged();
        }
    }

private:
    SimplifiedModel& mModel;
    std::vector<MemoryObserver*> mObservers;
};

/**
 * Runnable derived class to make the GUI independent of the simulation
 */
class SimplifiedModelRunner : public Runnable {
public:
    SimplifiedModelRunner(ProcessorModel* model)
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
        mModelRunner = new SimplifiedModelRunner(mModel);
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
    SimplifiedModelRunner* mModelRunner;
    ProcessorModel* mModel; // only reference, no ownership taken
};

/*-----------------------------------------------------------*/

static std::string _registerNames[32] = {
    "R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7",
    "R8", "R9", "R10", "R11", "R12", "R13", "R14", "R15",
    "R16", "R17", "R18", "R19", "R20", "R21", "R22", "R23",
    "R24", "R25", "R26", "R27", "R28", "R29", "R30", "R31"
};

SimplifiedModel::SimplifiedModel()
: mSimThread(new SimulatorThread(this))
{
    _reset();
    for (int i=0; i<65536; i++)
    {
        unsigned char byte = rand() & 0xff;
        mMemory[i] = byte;
    }
}

SimplifiedModel::~SimplifiedModel()
{
    delete mSimThread;
    mSimThread = 0;
}

void SimplifiedModel::_reset()
{
    mPC = 0;
    for (int i=0; i<32; i++)
    {
        mRegister[i]          = 0;
        mRegisterDirtyFlag[i] = false;
    }
}

std::string SimplifiedModel::getPluginName()
{
    return "SimplifiedModel";
}

void SimplifiedModel::run()
{
    for (int i=0; i<32; i++)
    {
        mRegisterDirtyFlag[i] = false;
    }
    mSimThread->start();
    SimulationStarted.sigmit();
}

void SimplifiedModel::step()
{
    mSimThread->stop();
    for (int i=0; i<32; i++)
    {
        mRegisterDirtyFlag[i] = false;
    }
    this->executeInstruction();
    SimulationStopped.sigmit();
}

void SimplifiedModel::stop()
{
    mSimThread->stop();
    SimulationStopped.sigmit();
}

void SimplifiedModel::reset()
{
    mSimThread->stop();
    _reset();
    SimulationReset.sigmit();
}

int SimplifiedModel::getPC() const
{
    return mPC;
}

bool SimplifiedModel::getRegister(int index, int& value) const
{
    if (index<0 || index>=32) {
        return false;
    }
    value = mRegister[index];
    return true;
}

bool SimplifiedModel::setRegister(int index, int value)
{
    if (index<0 || index>=32) {
        return false;
    }
    mRegister[index] = value;
    return true;
}

bool SimplifiedModel::getRegisterName(int index, std::string& name) const
{
    if (index<0 || index>=32) {
        return false;
    }
    name = _registerNames[index];
    return true;
}

int SimplifiedModel::getRegisterCount() const
{
    return 32;
}

bool SimplifiedModel::isRegisterDirty(int index) const
{
    if (index<32)
        return mRegisterDirtyFlag[index];
    else
        return false;
}

bool SimplifiedModel::getSmallMemoryByte(int address, unsigned char& value) const
{
    if (address<0 || address>=65536) {
        return false;
    }
    value = mMemory[address];
    return true;
}

bool SimplifiedModel::setSmallMemoryByte(int address, unsigned char value)
{
    if (address<0 || address>=65535) {
        return false;
    }
    mMemory[address] = value;
}

bool SimplifiedModel::getSmallMemoryHalfWord(int address, unsigned short& value) const
{
    if (address<0 || address>=65536-1) {
        return false;
    }
    value = (mMemory[address] << 8) | mMemory[address+1];
    return true;
}

bool SimplifiedModel::setSmallMemoryHalfWord(int address, unsigned short value)
{
    if (address<0 || address>=65535-1) {
        return false;
    }
    mMemory[address] = value >> 8;
    mMemory[address+1] = value & 0xff;
}

bool SimplifiedModel::getSmallMemoryWord(int address, unsigned int& value) const
{
    if (address<0 || address>=65536-3) {
        return false;
    }
    value = (mMemory[address] << 24) | (mMemory[address+1] << 16) |
            (mMemory[address+2] << 8) | mMemory[address+3];
    return true;
}

bool SimplifiedModel::setSmallMemoryWord(int address, unsigned int value)
{
    if (address<0 || address>=65535-3) {
        return false;
    }
    mMemory[address] = value >> 24;
    mMemory[address+1] = (value >> 16) & 0xff;
    mMemory[address+2] = (value >> 8) & 0xff;
    mMemory[address+3] = value & 0xff;
}

void SimplifiedModel::getMemoryList(std::vector<std::string>& list)
{
}

MemoryByteInterface* SimplifiedModel::getByteInterfaceByName(std::string& memory_name)
{
    return 0;
}

MemoryWordInterface* SimplifiedModel::getWordInterfaceByName(std::string& memory_name)
{
    return 0;
}

MemoryByteInterface* SimplifiedModel::getByteInterfaceByIndex(int index)
{
    return 0;
}

MemoryWordInterface* SimplifiedModel::getWordInterfaceByIndex(int index)
{
    return new SimplifiedModelMemoryProxy(*this);
}

// TODO: complete a simple DLX processor model
std::string SimplifiedModel::disassembleInstruction(int address) const
{
    std::ostringstream disassembly;
    uint32_t value;
    getSmallMemoryWord(address, value);
    int opcode1 = (value>>26) & 0x2f;

    int rd;
    int rs1 = (value>>21) & 0x1f;
    int rs2 = (value>>16) & 0x1f;
    int imm = value & 0xffff;
    if (0x0==opcode1)
        rd = (value>>11) & 0x1f;
    else
        rd = (value>>16) & 0x1f;

    switch (opcode1)
    {
        case 0: // R-type instruction
        {
            int opcode2 = value & 0x2f;
            switch (opcode2)
            {
                case 0x0:
                    if (0==value)
                        disassembly << "nop";
                    else
                        disassembly << "!!!Unknown instruction!!!";
                    break;
                case 0x04:
                    disassembly << "sll "
                        << _registerNames[rd] << ", "
                        << _registerNames[rs1] << ", "
                        << _registerNames[rs2];
                    break;
                case 0x06:
                    disassembly << "srl "
                        << _registerNames[rd] << ", "
                        << _registerNames[rs1] << ", "
                        << _registerNames[rs2];
                    break;
                case 0x07:
                    disassembly << "sra "
                        << _registerNames[rd] << ", "
                        << _registerNames[rs1] << ", "
                        << _registerNames[rs2];
                    break;
                case 0x20:
                    disassembly << "add "
                        << _registerNames[rd] << ", "
                        << _registerNames[rs1] << ", "
                        << _registerNames[rs2];
                    break;
                case 0x22:
                    disassembly << "sub "
                        << _registerNames[rd] << ", "
                        << _registerNames[rs1] << ", "
                        << _registerNames[rs2];
                    break;
                case 0x24:
                    disassembly << "and "
                        << _registerNames[rd] << ", "
                        << _registerNames[rs1] << ", "
                        << _registerNames[rs2];
                    break;
                case 0x25:
                    disassembly << "or "
                        << _registerNames[rd] << ", "
                        << _registerNames[rs1] << ", "
                        << _registerNames[rs2];
                    break;
                case 0x26:
                    disassembly << "xor "
                        << _registerNames[rd] << ", "
                        << _registerNames[rs1] << ", "
                        << _registerNames[rs2];
                    break;
                case 0x28:
                    disassembly << "seq "
                        << _registerNames[rd] << ", "
                        << _registerNames[rs1] << ", "
                        << _registerNames[rs2];
                    break;
                case 0x29:
                    disassembly << "sne "
                        << _registerNames[rd] << ", "
                        << _registerNames[rs1] << ", "
                        << _registerNames[rs2];
                    break;
                case 0x2a:
                    disassembly << "slt "
                        << _registerNames[rd] << ", "
                        << _registerNames[rs1] << ", "
                        << _registerNames[rs2];
                    break;
                case 0x2c:
                    disassembly << "sle "
                        << _registerNames[rd] << ", "
                        << _registerNames[rs1] << ", "
                        << _registerNames[rs2];
                    break;
                default:
                    disassembly << "!!!Unknown instruction!!!";
                    break;
            }
            break;
        }
        // I-type and J-type instructions
        case 0x02:
            disassembly << "jump "
                << imm;
            break;
        case 0x03:
            disassembly << "jal "
                << imm;
            break;
        case 0x04:
            disassembly << "beqz "
                << _registerNames[rs1] << ", "
                << imm;
            break;
        case 0x05:
            disassembly << "bnez "
                << _registerNames[rs1] << ", "
                << imm;
            break;
        case 0x08:
            disassembly << "addi "
                << _registerNames[rd] << ", "
                << _registerNames[rs1] << ", "
                << imm;
            break;
        case 0x0a:
            disassembly << "subi "
                << _registerNames[rd] << ", "
                << _registerNames[rs1] << ", "
                << imm;
            break;
        case 0x0c:
            disassembly << "andi "
                << _registerNames[rd] << ", "
                << _registerNames[rs1] << ", "
                << imm;
            break;
        case 0x0d:
            disassembly << "ori "
                << _registerNames[rd] << ", "
                << _registerNames[rs1] << ", "
                << imm;
            break;
        case 0x0e:
            disassembly << "xori "
                << _registerNames[rd] << ", "
                << _registerNames[rs1] << ", "
                << imm;
            break;
        case 0x0f:
            disassembly << "lhi "
                << _registerNames[rd] << ", "
                << imm;
            break;
        case 0x12:
            disassembly << "jr "
                << _registerNames[rs1];
            break;
        case 0x13:
            disassembly << "jalr "
                << _registerNames[rs1];
            break;
        case 0x14:
            disassembly << "slli "
                << _registerNames[rd] << ", "
                << _registerNames[rs1] << ", "
                << imm;
            break;
        case 0x16:
            disassembly << "srli "
                << _registerNames[rd] << ", "
                << _registerNames[rs1] << ", "
                << imm;
            break;
        case 0x17:
            disassembly << "srai "
                << _registerNames[rd] << ", "
                << _registerNames[rs1] << ", "
                << imm;
            break;
        case 0x18:
            disassembly << "seqi "
                << _registerNames[rd] << ", "
                << _registerNames[rs1] << ", "
                << imm;
            break;
        case 0x19:
            disassembly << "snei "
                << _registerNames[rd] << ", "
                << _registerNames[rs1] << ", "
                << imm;
            break;
        case 0x1a:
            disassembly << "slti "
                << _registerNames[rd] << ", "
                << _registerNames[rs1] << ", "
                << imm;
            break;
        case 0x1c:
            disassembly << "slei "
                << _registerNames[rd] << ", "
                << _registerNames[rs1] << ", "
                << imm;
            break;
        case 0x23:
            disassembly << "lw "
                << _registerNames[rd] << ", "
                << _registerNames[rs1] << ", "
                << imm;
            break;
        case 0x2b:
            disassembly << "sw "
                << _registerNames[rd] << ", "
                << _registerNames[rs1] << ", "
                << imm;
            break;
        default:
            disassembly << "!!!Unknown instruction!!!";
    }
    return disassembly.str();
}

void SimplifiedModel::executeInstruction()
{
    int instruction = mMemory[mPC];
    int nextPC = mPC + 1;

    int opcode1 = (instruction>>26) & 0x2f;

    int rd;
    int rs1 = (instruction>>21) & 0x1f;
    int rs2 = (instruction>>16) & 0x1f;
    int imm = instruction & 0xffff;
    if (0x0==opcode1)
        rd = (instruction>>11) & 0x1f;
    else
        rd = (instruction>>16) & 0x1f;

    switch (opcode1)
    {
        case 0: // R-type instruction
        {
            int opcode2 = instruction & 0x2f;
            switch (opcode2)
            {
                case 0x0:
                    if (0!=instruction)
                        std::cerr << "Unknown instruction";
                    break;
                case 0x04:
                    mRegister[rd] = mRegister[rs1] << mRegister[rs2];
                    mRegisterDirtyFlag[rd] = true;
                    break;
                case 0x06:
                    mRegister[rd] = mRegister[rs1] >> mRegister[rs2];
                    mRegisterDirtyFlag[rd] = true;
                    break;
                case 0x07:
                    mRegister[rd] = mRegister[rs1] >> mRegister[rs2];
                    mRegisterDirtyFlag[rd] = true;
                    break;
                case 0x20:
                    mRegister[rd] = mRegister[rs1] + mRegister[rs2];
                    mRegisterDirtyFlag[rd] = true;
                    break;
                case 0x21:
                    mRegister[rd] = mRegister[rs1] - mRegister[rs2];
                    mRegisterDirtyFlag[rd] = true;
                    break;
            }
            break;
        }
        case 0x02:
            nextPC = mPC + imm;
            break;
        case 0x04:
            //return "beqz";
            break;
        case 0x05:
            //return "bnez";
            break;
        case 0x0c:
            mRegister[rd] = mRegister[rs1] & imm;
            mRegisterDirtyFlag[rd] = true;
            break;
        case 0x0d:
            mRegister[rd] = mRegister[rs1] | imm;
            mRegisterDirtyFlag[rd] = true;
            break;
        case 0x0e:
            mRegister[rd] = mRegister[rs1] ^ imm;
            mRegisterDirtyFlag[rd] = true;
            break;
        case 0x0f:
            mRegister[rd] = imm;
            mRegisterDirtyFlag[rd] = true;
            break;
        case 0x12:
            nextPC = mRegister[rs1];
            break;
    }
    // wrap around in our 16bit memory
    nextPC = nextPC % ((1<<16)-1);
    this->mPC = nextPC;
}

int SimplifiedModel::parentIndex()
{
    return 0;
}

int SimplifiedModel::childCount()
{
    return 0;
}

SimulationObject* SimplifiedModel::getChild(int index)
{
    return NULL;
}

class SimplifiedModelFactory : public ProcessorControlFactory
{
public:
    virtual ProcessorControl* create()
    {
        return new SimplifiedModel();
    }
};

PLUGIN_API void registerPlugin(PluginManager* pm)
{
    PluginInformationBlock simplemodelinfo = {
        "Simplified DLX simulation model", 
        PROCESSOR_MODEL,
        "This simple processor model implements some of the DLX processor instructions but otherwise mainly servers to proof the concept of different processor models",
        "Silvan Wegmann",
        "swegmann@narf.ch",
        1, 0, 0,
        100,
        "13.2.2009"
    };
    pm->registerProcessorControl("SimplifiedModel", new SimplifiedModelFactory, simplemodelinfo);
}
