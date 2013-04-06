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

#ifndef _PLUGINMANAGER_H
#define _PLUGINMANAGER_H

#include "plugins/Plugin.h"
#include "plugins/ImageLoader.h"
#include "plugins/ScriptEngine.h"
#include "plugins/processor/ProcessorControl.h"
#include "Poco/Util/Subsystem.h"
#include <list>
#include <map>
#include <string>

#define STATIC_PLUGIN_INIT extern "C"

class PluginManager : public Poco::Util::Subsystem
{
public:
    /* generic plugin user interface */
    PluginManager();

    const char* name() const { return "PluginManager"; }
    void initialize(Poco::Util::Application& app);
    void uninitialize();

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
    void loadStaticPlugins();
    void loadDynamicPlugins(std::string& plugins_path);

    typedef std::map<std::string, ProcessorControlFactory*> ProcessorRegistry;
    ProcessorRegistry mProcessorFactories;
    typedef std::map<std::string, ImageLoaderFactory*> ImageLoaderRegistry;
    ImageLoaderRegistry mImageLoaderFactories;
    typedef std::map<std::string, ScriptEngineFactory*> ScriptEngineRegistry;
    ScriptEngineRegistry mScriptEngineFactories;
    std::vector<PluginInformationBlock> mPluginInfoList;

    static PluginManager* pInstance;
};

#endif
