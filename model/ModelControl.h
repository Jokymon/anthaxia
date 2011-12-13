#ifndef _MODELCONTROL_H
#define _MODELCONTROL_H

#include <string>
#include <vector>

class MemoryByteInterface;
class MemoryWordInterface;
class ProcessorControl;
class ProcessorControlUpdateInterface;

class ModelControl
{
public:
    ModelControl();
    ~ModelControl();

    void loadModel(const std::string& modelname);
    std::vector<std::string> getProcessorList() const;

    int getPC() const;
    bool getRegister(int index, int& value) const;
    bool setRegister(int index, int value);
    bool getRegisterName(int index, std::string& name) const;
    int getRegisterCount() const;
    bool isRegisterDirty(int index) const;
    bool getSmallMemoryByte(int address, unsigned char& value) const;
    bool setSmallMemoryByte(int address, unsigned char value);
    bool getSmallMemoryHalfWord(int address, unsigned short& value) const;
    bool setSmallMemoryHalfWord(int address, unsigned short value);
    bool getSmallMemoryWord(int address, unsigned int& value) const;
    bool setSmallMemoryWord(int address, unsigned int value);
    std::string disassembleInstruction(int address) const;

    void startSimulation();
    void stepSimulation();
    void stopSimulation();
    void resetSimulation();
    void loadImage(const std::string& fileName);
    void registerProcessorControlObserver(ProcessorControlUpdateInterface* observer);

    // Multi bus memory interface
    void getMemoryList(std::vector<std::string>& list);
    MemoryByteInterface* getByteInterfaceByName(std::string& memory_name);
    MemoryWordInterface* getWordInterfaceByName(std::string& memory_name);
    MemoryByteInterface* getByteInterfaceByIndex(int index);
    MemoryWordInterface* getWordInterfaceByIndex(int index);

    ProcessorControl* getProcessor();

private:
    ProcessorControl* mModel;
    std::vector<ProcessorControlUpdateInterface*> mProcessorControlObservers;

    void notifyModelChanged(ProcessorControl* newModel);
};

#endif
