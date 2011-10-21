#ifndef _SCRIPTENGINE_H
#define _SCRIPTENGINE_H

#include "plugins/Plugin.h"
#include <string>
#include <vector>

/* A class that describes the interface available for ScriptEngine plugins to
call functions of the processor simulator. An instance of this interface is
passed to the plugin to provide the corresponding functionality. */
class ScriptShellInterface
{
public:
    // TODO: maybe add different functions for stdout and stderr
    virtual void print(std::string output) =0; // function for output on the shell
};

class ScriptEngine
{
public:
    enum ExecutionResult { eER_SUCCESS, eER_MORE_INPUT_REQUIRED, eER_ERROR };
    virtual void init(ScriptShellInterface* shell) =0;
    virtual void shutdown() =0;
    virtual ExecutionResult executeCommand(std::string command) =0;
    virtual std::vector<std::string> autocompleteCommand(std::string command) =0;
    virtual std::string getNormalPrompt() =0;
    virtual std::string getMultilinePrompt() =0;   
};

typedef PluginFactory<ScriptEngine> ScriptEngineFactory;

#endif
