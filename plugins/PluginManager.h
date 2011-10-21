#ifndef _PLUGINMANAGER_H
#define _PLUGINMANAGER_H

#include "plugins/Plugin.h"
#include "plugins/ImageLoader.h"
#include "plugins/ScriptEngine.h"
#include "plugins/processor/ProcessorControl.h"
#include <list>
#include <map>
#include <string>

#define STATIC_PLUGIN_INIT extern "C"

class PluginManager
{
public:
    /* generic plugin user interface */
    static void init(std::string base_path);
    static PluginManager* getInstance();

    std::vector<PluginInformationBlock>::iterator pluginsBegin();
    std::vector<PluginInformationBlock>::iterator pluginsEnd();

    /* plugin specific user interface for processors */
    ProcessorControl* createProcessorControl(const std::string& objectType);
    std::list<std::string> getProcessorList() const;
    /* plugin specific user interface for image loaders */
    ImageLoader* createImageLoader(const std::string& suffix);
    /* plugin specific user interface for script engines */
    ScriptEngine* createScriptEngine(const std::string& name);


    /* plugin registration interface */
    void registerProcessorControl(std::string name, ProcessorControlFactory* factory, PluginInformationBlock info);
    void registerImageLoader(std::string suffix, ImageLoaderFactory* factory, PluginInformationBlock info);
    void registerScriptEngine(std::string name, ScriptEngineFactory* factory, PluginInformationBlock info);

private:
    PluginManager(std::string base_path);
    void loadStaticPlugins();
    void loadDynamicPlugins();

    typedef std::map<std::string, ProcessorControlFactory*> ProcessorRegistry;
    ProcessorRegistry mProcessorFactories;
    typedef std::map<std::string, ImageLoaderFactory*> ImageLoaderRegistry;
    ImageLoaderRegistry mImageLoaderFactories;
    typedef std::map<std::string, ScriptEngineFactory*> ScriptEngineRegistry;
    ScriptEngineRegistry mScriptEngineFactories;
    std::vector<PluginInformationBlock> mPluginInfoList;

    std::string mPluginsPath;
    
    static PluginManager* pInstance;
};

#endif
