#ifndef _PYTHONSHELL_H
#define _PYTHONSHELL_H

#include "plugins/ScriptEngine.h"

class PythonShell : public ScriptEngine
{
public:
    void init(ScriptShellInterface* shell);
    void shutdown();
    ScriptEngine::ExecutionResult executeCommand(std::string command);
    std::vector<std::string> autocompleteCommand(std::string command);
    std::string getNormalPrompt();
    std::string getMultilinePrompt();
private:
    ScriptShellInterface* mShell;
    void* python_locals;
};

class PluginManager;
PLUGIN_API void registerPlugin(PluginManager* pm);

#endif
