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

#include "PythonShell.h"

#include <Python.h>
#include <Poco/String.h>
#include "pyservicesystem.h"
#include "plugins/PluginManager.h"
#include <iostream>

using namespace std;

/****************************************************************/
/* Redirector Python class definition                           */

// Template for redirector objects
typedef struct {
    PyObject_HEAD
    ScriptShellInterface* shell;
} redirector_redirectorObject;

// Redirector functions
static PyObject* redirector_init(PyObject *, PyObject *)
{
    Py_RETURN_NONE;
}

static PyObject* redirector_write(redirector_redirectorObject* self, PyObject *args)
{
    char* output;

    if (!PyArg_ParseTuple(args, "s", &output))
    {
        return NULL;
    }

    self->shell->print(output);
    Py_RETURN_NONE;
}

// Redirector methods
static PyMethodDef redirector_methods[] =
{
    {"__init__", redirector_init, METH_VARARGS,
        "initialize the stdout/err redirector"},
    {"write", (PyCFunction)redirector_write, METH_VARARGS,
        "implement the write method to redirect stdout/err"},
    {NULL,NULL,0,NULL},
};

// Redirector class description for Python
static PyTypeObject redirector_redirectorType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "redirector._redirector",  /*tp_name*/
    sizeof(redirector_redirectorObject), /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    0,                         /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT,        /*tp_flags*/
    "redirector objects",      /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    redirector_methods,        /* tp_methods */
};

/* end of redirector class definition                           */
/****************************************************************/

static PyMethodDef ModuleMethods[] = { 
    {NULL,NULL,0,NULL} 
};

void initredirector()
{
    PyObject* m;

    redirector_redirectorType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&redirector_redirectorType) < 0)
    {
        std::cout << "Making the redirector ready failed" << std::endl;
        return;
    }

    m = Py_InitModule3("redirector", ModuleMethods,
                       "Class for output redirection");
    Py_INCREF(&redirector_redirectorType);
    PyModule_AddObject(m, "_redirector", (PyObject*)&redirector_redirectorType);
}
/****************************************************************/

void PythonShell::init(ScriptShellInterface* shell)
{
    mShell = shell;
    Py_Initialize();

    // Create locals for interpreting Python commands
    // TODO: Proper cleanup after/while shutdown
    python_locals = PyDict_New();

    PyObject* sys_module = PyImport_ImportModule("sys");
    // Add path of Python scripts
    PyObject* sys_path = PyObject_GetAttrString(sys_module, "path");
    // TODO: need to get the plugin directory feed in from somewhere else
    PyObject* py_scripts_path = PyString_FromString("C:\\Silvan_Home\\proc_sim_plugin_console\\plugins\\pythonshell");
    PyList_Append(sys_path, py_scripts_path);

    initredirector();

    // create a redirector object to be used as stdout and stderr
    redirector_redirectorObject* redir = PyObject_New(redirector_redirectorObject, &redirector_redirectorType);
    redir->shell = shell;
    PyObject_SetAttrString(sys_module, "stdout", (PyObject*)redir);
    PyObject_SetAttrString(sys_module, "stderr", (PyObject*)redir);
    if (PyErr_Occurred ())
    {
        PyErr_Print ();
    }
    initservicesystem();

    // cleanup before we leave the function
    Py_XDECREF(sys_module);
}

void PythonShell::shutdown()
{
}

ScriptEngine::ExecutionResult PythonShell::executeCommand(string command)
{
    ScriptEngine::ExecutionResult success = ScriptEngine::eER_SUCCESS;

    PyObject *err=NULL, *err1=NULL, *err2=NULL;
    PyObject *code=NULL, *code1=NULL, *code2=NULL;
    PyObject *ptype=NULL, *ptrace=NULL, *tmp1=NULL, *tmp2=NULL;

    PyObject* py_result=NULL;
    PyObject* globals=NULL;

    PyCompilerFlags co_flags;
    co_flags.cf_flags = PyCF_DONT_IMPLY_DEDENT;

    // implementation of _maybe_compile() in C
    //----------------------------------------

    // Check for source consisting only of blank lines
    if (Poco::trim(command)=="")
    {
        goto cleanup;
    }

    code = Py_CompileStringFlags(command.c_str(), "<stdin>", Py_single_input, &co_flags);
    if (code==NULL)
    {
        if (!PyErr_ExceptionMatches(PyExc_SyntaxError))
        {
            PyErr_Print();
            success = ScriptEngine::eER_ERROR;
            goto cleanup;
        }
        PyErr_Fetch(&tmp1, &err, &tmp2);
        Py_XDECREF(tmp1);   // we don't need this result
        Py_XDECREF(tmp2);   // we don't need this result
        PyErr_Clear();
    }

    command.append("\n");
    code1 = Py_CompileStringFlags(command.c_str(), "<stdin>", Py_single_input, &co_flags);
    if (code1==NULL)
    {
        if (!PyErr_ExceptionMatches(PyExc_SyntaxError))
        {
            PyErr_Print();
            success = ScriptEngine::eER_ERROR;
            goto cleanup;
        }
        PyErr_Fetch(&ptype, &err1, &ptrace);
        Py_XDECREF(ptype);  // we don't need this result
        Py_XDECREF(ptrace); // we don't need this result
        PyErr_Clear();
    }

    command.append("\n");
    code2 = Py_CompileStringFlags(command.c_str(), "<stdin>", Py_single_input, &co_flags);
    if (code2==NULL)
    {
        if (!PyErr_ExceptionMatches(PyExc_SyntaxError))
        {
            PyErr_Print();
            success = ScriptEngine::eER_ERROR;
            goto cleanup;
        }
        PyErr_Fetch(&tmp1, &err2, &tmp2);
        Py_XDECREF(tmp1);   // we don't need this result
        Py_XDECREF(tmp2);   // we don't need this result
        PyErr_Clear();
    }

    PyObject *r1, *r2;
    r1 = PyObject_Repr(err1);   r2 = PyObject_Repr(err2);
    if ((code1==NULL) and (PyObject_Compare(r1, r2)==0))
    {
        // print exception of err1
        PyErr_Restore(ptype, err1, ptrace);
        PyErr_Print();
        std::cout << "Ok, we have a real syntax error" << std::endl;
        success = ScriptEngine::eER_ERROR;
        goto cleanup;
    } else if (code==NULL) {
        success = ScriptEngine::eER_MORE_INPUT_REQUIRED;
        goto cleanup;
    }

    // Execute a complete command in the __builtins__ environment
    globals = PyDict_New();
    PyDict_SetItemString( globals, "__builtins__", PyEval_GetBuiltins() );

    py_result = PyEval_EvalCode((PyCodeObject*) code, globals, (PyObject*)python_locals);
    if (PyErr_Occurred())
    {
        PyErr_Print();
        success = ScriptEngine::eER_ERROR;
    } 

cleanup:
    Py_XDECREF(err); Py_XDECREF(err1); Py_XDECREF(err2);
    Py_XDECREF(code); Py_XDECREF(code1); Py_XDECREF(code2);
    Py_XDECREF(ptype); Py_XDECREF(ptrace);
    Py_XDECREF(py_result);
    return success;
}

vector<string> PythonShell::autocompleteCommand(string command)
{
}

string PythonShell::getNormalPrompt()
{
    return ">>> ";
}

string PythonShell::getMultilinePrompt()
{
    return "... ";
}

class PythonShellFactory : public ScriptEngineFactory
{
public:
    virtual ScriptEngine* create()
    {
        return new PythonShell();
    }
};

PLUGIN_API void registerPlugin(PluginManager* pm)
{
    PluginInformationBlock pythonshellinfo = {
        "Python Shell Plugin",
        SCRIPT_ENGINE,
        "A Python script engine plugin integrating Python into the simulator",
        "Silvan Wegmann",
        "swegmann@narf.ch",
        1, 1, 0,
        165,
        "18.11.2010"
    };
    pm->registerScriptEngine("PythonShell", new PythonShellFactory, pythonshellinfo);
}
