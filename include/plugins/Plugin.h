#ifndef _PLUGIN_H
#define _PLUGIN_H

#include <string>

/**
 * Factory template for plugin instances. Plugins register instances of these
 * factories with the PluginManager to allow the application to create
 * instances of that plugin.
 */
template<typename Interface>
class PluginFactory
{
public:
    virtual Interface* create() =0;
};

class PluginManager;
/**
 * Prototype of functions that the PluginManager will call in plugins
 */
typedef void (*registerPluginFunction)(PluginManager* pm);

/**
 * A structure to contain all information about a specific plugin, such as
 * author, support contact, description and version
 */
struct PluginInformationBlock
{
    std::string name;           // name of the plugin
    std::string category;       // image loader, processor mode, ... this is set by the plugin manager
    std::string description;    // description of the plugin: what does it do, etc
    std::string author;         // who wrote this plugin: one or multiple names separated by ,
    std::string support;        // webpage, E-Mail address or other means for contacting the author(s) of this plugin
    int         major, minor, patch; // version number of this plugin
    int         build;          // build number of this plugin
    std::string builddate;      // build date of this plugin
};

/* supported categories */
#define IMAGE_LOADER "Image loader"
#define PROCESSOR_MODEL "Processor model"
#define SCRIPT_ENGINE "Script engine"

#ifdef _WIN32

#ifdef BUILD_DLL
#define PLUGIN_API extern "C" __declspec(dllexport)
#else
#define PLUGIN_API extern "C" __declspec(dllimport)
#endif

#else

#define PLUGIN_API extern "C"

#endif

#endif
