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

#include "ModelControl.h"
#include "Logging/Logging.h"
#include "plugins/processor/ProcessorControl.h"
#include "plugins/processor/ProcessorControlUpdateInterface.h"
#include "plugins/PluginManager.h"

#include "Poco/Util/Application.h"
#include "Poco/Path.h"

#include <iostream>

ModelControl* ModelControl::pInstance = 0;
DEFINE_STATIC_LOGGER("core.ModelControl");

ModelControl* ModelControl::getInstance()
{
    if (pInstance == 0)
    {
        pInstance = new ModelControl;
    }
    return pInstance;
}

ModelControl::ModelControl()
: mModel(0)
{
}

ModelControl::~ModelControl()
{
    if (mModel)
    {
        delete mModel;
        mModel = 0;
    }
}

void ModelControl::loadModel(const std::string& modelname)
{
    LOG_DEBUG("Trying to create model instance " << modelname);
    PluginManager& plugin_manager = Poco::Util::Application::instance().getSubsystem<PluginManager>();
    ProcessorControl* temp = plugin_manager.createProcessorControl(modelname);
    if (temp)
    {
        LOG_DEBUG("Successfully created instance " << modelname);
        mModel = temp;
        this->notifyModelChanged(mModel);
    } else
    {
        LOG_DEBUG("Could not create model " << modelname);
    }
}

std::vector<std::string> ModelControl::getProcessorList() const
{
    std::vector<std::string> processorModels;

    PluginManager& plugin_manager = Poco::Util::Application::instance().getSubsystem<PluginManager>();
    std::list<std::string> l = plugin_manager.getProcessorList();
    for (std::list<std::string>::const_iterator it=l.begin();
         it != l.end();
         ++it)
    {
        processorModels.push_back(std::string((*it).c_str()));
    }
    return processorModels;
}

void ModelControl::startSimulation()
{
    if (mModel)
    {
        LOG_DEBUG("Starting the simulation");
        mModel->run();
    }
}

void ModelControl::stepSimulation()
{
    if (mModel)
    {
        LOG_DEBUG("Executing one step of the simulation");
        mModel->step();
    }
}

void ModelControl::stopSimulation()
{
    if (mModel)
    {
        LOG_DEBUG("Stopping the simulation");
        mModel->stop();
    }
}

void ModelControl::resetSimulation()
{
    if (mModel)
    {
        LOG_DEBUG("Reseting the simulation");
        mModel->reset();
    }
}

int ModelControl::getPC() const
{
    if (mModel)
        return mModel->getPC();
    else
        return 0;
}

bool ModelControl::getRegister(int index, int& value) const
{
    if (mModel)
        return mModel->getRegister(index, value);
    else
        return false;
}

bool ModelControl::setRegister(int index, int value)
{
    if (mModel)
        return mModel->setRegister(index, value);
    else
        return false;
}

bool ModelControl::getRegisterName(int index, std::string& name) const
{
    if (mModel)
        return mModel->getRegisterName(index, name);
    else
        return false;
}

int ModelControl::getRegisterCount() const
{
    if (mModel)
        return mModel->getRegisterCount();
    else
        return 0;
}

bool ModelControl::isRegisterDirty(int index) const
{
    if (mModel)
        return mModel->isRegisterDirty(index);
    else
        return false;
}

bool ModelControl::setSmallMemoryByte(int address, unsigned char value)
{
    if (mModel)
        return mModel->setSmallMemoryByte(address, value);
    else
        return false;
}

bool ModelControl::getSmallMemoryByte(int address, unsigned char& value) const
{
    if (mModel)
        return mModel->getSmallMemoryByte(address, value);
    else
        return false;
}

bool ModelControl::setSmallMemoryHalfWord(int address, unsigned short value)
{
    if (mModel)
        return mModel->setSmallMemoryHalfWord(address, value);
    else
        return false;
}

bool ModelControl::getSmallMemoryHalfWord(int address, unsigned short& value) const
{
    if (mModel)
        return mModel->getSmallMemoryHalfWord(address, value);
    else
        return false;
}

bool ModelControl::setSmallMemoryWord(int address, unsigned int value)
{
    if (mModel)
        return mModel->setSmallMemoryWord(address, value);
    else
        return false;
}

bool ModelControl::getSmallMemoryWord(int address, unsigned int& value) const
{
    if (mModel)
        return mModel->getSmallMemoryWord(address, value);
    else
        return false;
}

std::string ModelControl::disassembleInstruction(int address) const
{
    return mModel->disassembleInstruction(address).c_str();
}

void ModelControl::loadImage(const std::string& fileName)
{
    Poco::Path path(fileName);
    PluginManager& plugin_manager = Poco::Util::Application::instance().getSubsystem<PluginManager>();
    ImageLoader* loader = plugin_manager.createImageLoader(path.getExtension());
    if (loader)
    {
        LOG_DEBUG("Found an image loader plugin for suffix " << path.getExtension());
        MemoryByteInterface* interface = this->getByteInterfaceByIndex(0);
        loader->loadImage(fileName, interface);
    } else
    {
        LOG_ERROR("Couldn't find an image loader for this extension");
    }
}

void ModelControl::registerProcessorControlObserver(ProcessorControlUpdateInterface* observer)
{
    LOG_DEBUG("Adding an observer to the model control observer list");
    mProcessorControlObservers.push_back(observer);
}

void ModelControl::getMemoryList(std::vector<std::string>& list)
{
}

void ModelControl::notifyModelChanged(ProcessorControl* newModel)
{
    LOG_DEBUG("Informing all observers about the new model");
    for (std::vector<ProcessorControlUpdateInterface*>::iterator iter = mProcessorControlObservers.begin();
         iter != mProcessorControlObservers.end();
         iter++)
    {
        (*iter)->handleModelChanged(newModel);
    }
}

MemoryByteInterface* ModelControl::getByteInterfaceByName(std::string& memory_name)
{
    if (mModel)
        return mModel->getByteInterfaceByName(memory_name);
    else
        return 0;
}

MemoryWordInterface* ModelControl::getWordInterfaceByName(std::string& memory_name)
{
    if (mModel)
        return mModel->getWordInterfaceByName(memory_name);
    else
        return 0;
}

MemoryByteInterface* ModelControl::getByteInterfaceByIndex(int index)
{
    if (mModel)
        return mModel->getByteInterfaceByIndex(index);
    else
        return 0;
}

MemoryWordInterface* ModelControl::getWordInterfaceByIndex(int index)
{
    if (mModel)
        return mModel->getWordInterfaceByIndex(index);
    else
        return 0;
}

