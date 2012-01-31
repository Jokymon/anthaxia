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

#include "PsModelControl.h"
#include "Logging/Logging.h"
#include "plugins/processor/ProcessorControl.h"
#include "plugins/PluginManager.h"
#include "elfloader/PsElfLoader.h"

#include <iostream>

DEFINE_STATIC_LOGGER("gui.PsModelControl");

PsModelControl::PsModelControl(ModelControl& mc)
: mModelControl(mc)
{
    LOG_DEBUG("Registering myself at the model control");
    mModelControl.registerProcessorControlObserver(this);
}

PsModelControl::~PsModelControl()
{ }

void PsModelControl::loadModel(const QString& modelname)
{
    LOG_DEBUG("Loading model " << modelname.toStdString());
    mModelControl.loadModel(modelname.toStdString());
}

QStringList PsModelControl::getProcessorList() const
{
    QStringList processorModels;

    std::list<std::string> l = 
        PluginManager::getInstance()->getProcessorList();
    for (std::list<std::string>::const_iterator it=l.begin();
         it != l.end();
         ++it)
    {
        processorModels.push_back(QString((*it).c_str()));
    }
    return processorModels;
}

void PsModelControl::startSimulation()
{
    mModelControl.startSimulation();
}

void PsModelControl::stepSimulation()
{
    mModelControl.stepSimulation();
}

void PsModelControl::stopSimulation()
{
    mModelControl.stopSimulation();
}

void PsModelControl::resetSimulation()
{
    mModelControl.resetSimulation();
}

int PsModelControl::getPC() const
{
    return mModelControl.getPC();
}

bool PsModelControl::getRegister(int index, int& value) const
{
    return mModelControl.getRegister(index, value);
}

bool PsModelControl::setRegister(int index, int value)
{
    return mModelControl.setRegister(index, value);
}

bool PsModelControl::getRegisterName(int index, std::string& name) const
{
    return mModelControl.getRegisterName(index, name);
}

int PsModelControl::getRegisterCount() const
{
    return mModelControl.getRegisterCount();
}

bool PsModelControl::isRegisterDirty(int index) const
{
    return mModelControl.isRegisterDirty(index);
}

bool PsModelControl::setSmallMemoryByte(int address, unsigned char value)
{
    return mModelControl.setSmallMemoryByte(address, value);
}

bool PsModelControl::getSmallMemoryByte(int address, unsigned char& value) const
{
    return mModelControl.getSmallMemoryByte(address, value);
}

bool PsModelControl::setSmallMemoryHalfWord(int address, unsigned short value)
{
    return mModelControl.setSmallMemoryHalfWord(address, value);
}

bool PsModelControl::getSmallMemoryHalfWord(int address, unsigned short& value) const
{
    return mModelControl.getSmallMemoryHalfWord(address, value);
}

bool PsModelControl::setSmallMemoryWord(int address, unsigned int value)
{
    return mModelControl.setSmallMemoryWord(address, value);
}

bool PsModelControl::getSmallMemoryWord(int address, unsigned int& value) const
{
    return mModelControl.getSmallMemoryWord(address, value);
}

QString PsModelControl::disassembleInstruction(int address) const
{
    return mModelControl.disassembleInstruction(address).c_str();
}

void PsModelControl::handleSimulationStarted()
{
    emit simulationStarted();
}

void PsModelControl::handleSimulationStopped()
{
    emit simulationStopped();
}

void PsModelControl::handleSimulationReset()
{
    emit simulationReset();
}

void PsModelControl::handleLoadImage(const QString& fileName)
{
    mModelControl.loadImage(fileName.toStdString());
}

void PsModelControl::handleModelChanged(ProcessorControl* newModel)
{
    LOG_DEBUG("Model has changed, PsModelControl is registered as simulation observer");
    newModel->SimulationStarted.connect(this, &PsModelControl::handleSimulationStarted);
    newModel->SimulationStopped.connect(this, &PsModelControl::handleSimulationStopped);
    emit modelChanged();
}

void PsModelControl::getMemoryList(QStringList& list)
{
}

MemoryByteInterface* PsModelControl::getByteInterfaceByName(QString& memory_name)
{
    std::string mem_name = memory_name.toStdString();
    return mModelControl.getByteInterfaceByName(mem_name);
}

MemoryWordInterface* PsModelControl::getWordInterfaceByName(QString& memory_name)
{
    std::string mem_name = memory_name.toStdString();
    return mModelControl.getWordInterfaceByName(mem_name);
}

MemoryByteInterface* PsModelControl::getByteInterfaceByIndex(int index)
{
    return mModelControl.getByteInterfaceByIndex(index);
}

MemoryWordInterface* PsModelControl::getWordInterfaceByIndex(int index)
{
    return mModelControl.getWordInterfaceByIndex(index);
}

ProcessorControl* PsModelControl::getProcessor()
{
    return mModelControl.getProcessor();
}

#include "PsModelControl.moc"
