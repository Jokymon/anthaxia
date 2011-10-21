#include "PluginManager.h"
#include "static_plugins.h"
#include "Logging/Logging.h"
#include "platform/DynamicLibrary.h"
#include "Poco/DirectoryIterator.h"
#include "Poco/Glob.h"
#include "Poco/Path.h"
#include <iostream>
#include <string>
#include <set>

DEFINE_STATIC_LOGGER("core.PluginManager");

PluginManager* PluginManager::pInstance = 0;

PluginManager::PluginManager(std::string base_path)
: mPluginsPath(base_path)
{
    this->loadStaticPlugins();
    this->loadDynamicPlugins();
}

void PluginManager::loadStaticPlugins()
{
    LOG_DEBUG("Loading static plugins");
    LOAD_STATIC_PLUGINS(this);
}

void PluginManager::loadDynamicPlugins()
{
    LOG_DEBUG("Loading dynamic plugins");
    Poco::Path plugin_directory = Poco::Path::forDirectory(mPluginsPath);
    plugin_directory.popDirectory();
    plugin_directory.pushDirectory("plugins");
    LOG_DEBUG("Plugin directory: " << plugin_directory.toString());
    std::set<std::string> dll_files;
    Poco::Glob::glob(plugin_directory.append(DynamicLibrary::DL_PREFIX+
                                             "*."+
                                             DynamicLibrary::DL_SUFFIX), 
                     dll_files);

    for (std::set<std::string>::iterator it = dll_files.begin(); 
         it!=dll_files.end(); 
         ++it)
    {
        std::string plugin_file = *it;
        LOG_DEBUG("Loading library " << plugin_file);
        DynamicLibrary* dl = DynamicLibrary::load(plugin_file);
        registerPluginFunction registerPlugin = 
            (registerPluginFunction)dl->getSymbol("registerPlugin");
        if (registerPlugin)
        {
            LOG_DEBUG("Initialize Plugin " << *it);
            registerPlugin(this);
        } else
        {
            LOG_WARN("Couldn't find registerPlugin for " << *it);
        }
    }

}

void PluginManager::init(std::string base_path)
{
    pInstance = new PluginManager(base_path);
}

PluginManager* PluginManager::getInstance()
{
    if (pInstance == 0)
    {
        pInstance = new PluginManager(".");
    }
    return pInstance;
}

std::vector<PluginInformationBlock>::iterator PluginManager::pluginsBegin()
{
    return mPluginInfoList.begin();
}

std::vector<PluginInformationBlock>::iterator PluginManager::pluginsEnd()
{
    return mPluginInfoList.end();
}

/****************************************************************************************/
/* plugin functions for dealing with processor plugins                                  */
void PluginManager::registerProcessorControl(std::string name, ProcessorControlFactory* factory, PluginInformationBlock info)
{
    LOG_DEBUG("Registering processor model named " << name);
    mProcessorFactories[name] = factory;
    mPluginInfoList.push_back(info);
}

ProcessorControl* PluginManager::createProcessorControl(const std::string& objectType)
{
    if (mProcessorFactories.find(objectType) != mProcessorFactories.end())
    {
        return mProcessorFactories[objectType]->create();
    } else
    {
        LOG_ERROR("Couldn't find processor plugin named " << objectType);
        return 0;
    }
}

std::list<std::string> PluginManager::getProcessorList() const
{
    std::list<std::string> processorList;
    for (ProcessorRegistry::const_iterator it=mProcessorFactories.begin();
         it != mProcessorFactories.end();
         ++it)
    {
        processorList.push_back( (*it).first );
    }
    return processorList;
}

/****************************************************************************************/
/* plugin functions for dealing with image loader plugins                               */
void PluginManager::registerImageLoader(std::string suffix, ImageLoaderFactory* factory, PluginInformationBlock info)
{
    LOG_DEBUG("Registering image loader for suffix " << suffix);
    mImageLoaderFactories[suffix] = factory;
    mPluginInfoList.push_back(info);
}

ImageLoader* PluginManager::createImageLoader(const std::string& suffix)
{
    if (mImageLoaderFactories.find(suffix) != mImageLoaderFactories.end())
    {
        return mImageLoaderFactories[suffix]->create();
    } else
    {
        LOG_ERROR("Couldn't find image loader plugin for suffix " << suffix);
        return 0;
    }
}

/****************************************************************************************/
/* plugin functions for dealing with script engine plugins                              */

void PluginManager::registerScriptEngine(std::string name, ScriptEngineFactory* factory, PluginInformationBlock info)
{
    LOG_DEBUG("Registering script engine " << name);
    mScriptEngineFactories[name] = factory;
    mPluginInfoList.push_back(info);
}

ScriptEngine* PluginManager::createScriptEngine(const std::string& name)
{
    if (mScriptEngineFactories.find(name) != mScriptEngineFactories.end())
    {
        return mScriptEngineFactories[name]->create();
    } else
    {
        LOG_ERROR("Couldn't find script engine plugin with name" << name);
        return 0;
    }
}


