#ifndef _PSMODELCONTROL_H
#define _PSMODELCONTROL_H

#include <QObject>
#include <QStringList>
#include "sigslot/sigslot.h"
#include "model/ModelControl.h"
#include "plugins/processor/ProcessorControlUpdateInterface.h"

class MemoryByteInterface;
class MemoryWordInterface;
class ModelControl;

class PsModelControl : public QObject, public ProcessorControlUpdateInterface, public sigslot::has_slots<>
{
Q_OBJECT
public:
    PsModelControl(ModelControl& mc);
    ~PsModelControl();

    void loadModel(const QString& modelname);
    QStringList getProcessorList() const;

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
    QString disassembleInstruction(int address) const;

    void handleSimulationStarted();
    void handleSimulationStopped();
    void handleSimulationReset();
    virtual void handleLoadImage(const QString& fileName);

    virtual void handleModelChanged(ProcessorControl* newModel);

    // Multi bus memory interface
    void getMemoryList(QStringList& list);
    MemoryByteInterface* getByteInterfaceByName(QString& memory_name);
    MemoryWordInterface* getWordInterfaceByName(QString& memory_name);
    MemoryByteInterface* getByteInterfaceByIndex(int index);
    MemoryWordInterface* getWordInterfaceByIndex(int index);

signals:
    void simulationStarted();
    void simulationStopped();
    void simulationReset();
    void modelChanged();
public slots:
    void startSimulation();
    void stepSimulation();
    void stopSimulation();
    void resetSimulation();
private:
    ModelControl& mModelControl;
};

#endif
