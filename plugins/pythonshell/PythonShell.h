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
