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

#include <Python.h>
#include <structmember.h>
#include "servicesystem/servicesystem.h"
#include "servicesystem/marshalling.h"

/********************************************************************/

static PyObject* UnknownServiceError;
static PyObject* MarshallingError;

/********************************************************************/

typedef struct {
    PyObject_HEAD
    ProxyObject po;
    PyObject* dict;
} PyProxy;

static PyObject *
PyProxy_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyProxy *self;
    
    self = (PyProxy *)type->tp_alloc(type, 0);
    self->dict = PyDict_New();
    if (self->dict==NULL)
    {
        Py_DECREF(self);
        return NULL;
    }
    return (PyObject *)self;
}

static PyObject *
PyProxy_call_method(PyProxy *self, PyObject *args)
{
    char* method_name;

    int arg_size = PyTuple_Size(args);
    /* Checking the first argument that should give us the name, the signature and the return value to call */
    if (arg_size<1)
    {
        PyErr_Format(PyExc_TypeError, "call_method takes at least 1 argument (%u given)", arg_size);
        return NULL;
    }
    PyObject* args0 = PyTuple_GET_ITEM(args, 0);
    if (!PyString_Check(args0))
    {
        PyErr_SetString(PyExc_TypeError, "first argument must be a string given the method to call");
        return NULL;
    }
    method_name = PyString_AS_STRING(args0);

    /* Now create the marshalled objects for calling the function */
    MarshaledData* arguments = create_method_call();
    MarshaledData* result;
    /* Add parameters of the Python function to the marshalled object */
    int i;
    for (i = 1; i<arg_size; i++)
    {
        PyObject* item = PyTuple_GET_ITEM(args, i);
        if (PyInt_Check(item))
        {
            append_int(arguments, PyInt_AS_LONG(item));
        } else if (PyFloat_Check(item))
        {
            append_double(arguments, PyFloat_AS_DOUBLE(item));
        } else if (PyString_Check(item))
        {
            append_string(arguments, PyString_AS_STRING(item));
        } else
        {
            char warning_msg[50];
            snprintf(warning_msg, 50, "Python type of argument %u is not supported", i);
            PyErr_WarnEx(NULL, warning_msg, 1);
        }
    }

    PyObject* pyresult = NULL;
    CallResult res = call_method(self->po, method_name, arguments, &result);
    switch (res)
    {
        case CALL_OK:
            {
                MarshallParser* parser = create_parser(result);
                ServiceTypes typ = get_next_type(parser);
                int int_result;
                double double_result;
                char* string_result;
                switch(typ)
                {
                    case TYPE_INT:
                        parse_int(parser, &int_result);
                        pyresult = Py_BuildValue("i", int_result);
                        break;
                    case TYPE_DOUBLE:
                        parse_double(parser, &double_result);
                        pyresult = Py_BuildValue("d", double_result);
                        break;
                    case TYPE_STRING:
                        parse_string(parser, &string_result);
                        pyresult = Py_BuildValue("s", &string_result);
                        break;
                    case TYPE_NONE:
                        Py_INCREF(Py_None);
                        pyresult = Py_None;
                        break;
                }
                free_parser(parser);
            }
            break;
        case CALL_UNKNOWN_NAME:
            PyErr_Format(PyExc_AttributeError, "Instance has no attribute '%s'", method_name);
            break;
        case CALL_UNKNOWN_SIGNATURE:
            PyErr_SetString(PyExc_TypeError, "Invalid type and/or number of arguments");
            break;
        case CALL_MARSHALLING_ERROR:
            PyErr_SetString(MarshallingError, "An error happend during marshalling/demarshalling of the function call");
            break;
    }
//done:
    free_method_call(arguments);
    return pyresult;
}

static PyObject *
PyProxy_get_method_list(PyProxy *self, PyObject *args)
{
    MethodList* list = get_method_list(self->po);
    PyObject* pylist = PyList_New(list->size);
    PyObject* entry;
    int i;
    for (i=0; i<list->size; i++)
    {
        entry = Py_BuildValue("(sss)", list->methods[i]->name, list->methods[i]->signature, list->methods[i]->returntype);
        PyList_SetItem(pylist, i, entry);
    }
    return pylist;
}

static int
PyProxy_init(PyProxy *self, PyObject *args, PyObject *kwds)
{
    self->po = NULL;
    return 0;
}


static PyMethodDef PyProxy_methods[] = {
    {"call_method", (PyCFunction)PyProxy_call_method, METH_VARARGS,
     PyDoc_STR("call a method of this proxy object")},
    {"get_method_list", (PyCFunction)PyProxy_get_method_list, METH_NOARGS,
     PyDoc_STR("get a list of available methods")},
    {NULL,	NULL},
};


static PyMemberDef PyProxy_members[] = {
    {"__dict__", T_OBJECT_EX, offsetof(PyProxy, dict), 0, ""},
    {NULL}  /* Sentinel */
};


static PyTypeObject PyProxyType = {
    PyObject_HEAD_INIT(NULL)
    0,                       /* ob_size */
    "servicesystem.PyProxy", /* tp_name */
    sizeof(PyProxy),         /* tp_basicsize */
    0,                       /* tp_itemsize */
    0,                       /* tp_dealloc */
    0,                       /* tp_print */
    0,                       /* tp_getattr */
    0,                       /* tp_setattr */
    0,                       /* tp_compare */
    0,                       /* tp_repr */
    0,                       /* tp_as_number */
    0,                       /* tp_as_sequence */
    0,                       /* tp_as_mapping */
    0,                       /* tp_hash */
    0,                       /* tp_call */
    0,                       /* tp_str */
    0,                       /* tp_getattro */
    0,                       /* tp_setattro */
    0,                       /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT |
      Py_TPFLAGS_BASETYPE,   /* tp_flags */
    0,                       /* tp_doc */
    0,                       /* tp_traverse */
    0,                       /* tp_clear */
    0,                       /* tp_richcompare */
    0,                       /* tp_weaklistoffset */
    0,                       /* tp_iter */
    0,                       /* tp_iternext */
    PyProxy_methods,         /* tp_methods */
    PyProxy_members,         /* tp_members */
    0,                       /* tp_getset */
    0,                       /* tp_base */
    0,                       /* tp_dict */
    0,                       /* tp_descr_get */
    0,                       /* tp_descr_set */
    offsetof(PyProxy, dict), /* tp_dictoffset */
    (initproc)PyProxy_init,  /* tp_init */
    0,                       /* tp_alloc */
    PyProxy_new,             /* tp_new */
};

/********************************************************************/

static PyObject*
py_get_service_proxy(PyObject *self, PyObject *args)
{
    const char* service_name;

    if (!PyArg_ParseTuple(args, "s", &service_name))
    {
        return NULL;
    }

    ProxyObject po = get_service(service_name);
    if (po==NULL)
    {
        PyErr_Format(UnknownServiceError, "A service named '%s' is not available", service_name);
        return NULL;
    }

    PyObject* mod = PyImport_ImportModule("servicesystem");
    PyObject* cls = PyObject_GetAttrString(mod,"PyProxy");
    PyObject* inst = PyObject_CallFunctionObjArgs(cls,NULL);

    ((PyProxy*)inst)->po = po;

    Py_DECREF(mod);
    Py_DECREF(cls);
    return inst;
}

static PyObject*
py_get_service_list(PyObject *self, PyObject *args)
{
    ServiceList* service_list = get_service_list();
    PyObject* result = PyList_New(service_list->size);
    PyObject* entry;
    int i;
    for (i=0; i<service_list->size; i++)
    {
        entry = Py_BuildValue("s", service_list->services[i]);
        PyList_SetItem(result, i, entry);
    }
    return result;
}

static PyMethodDef moduleMethods[] = {
    {"get_service_proxy", py_get_service_proxy, METH_VARARGS, ""},
    {"get_service_list", py_get_service_list, METH_VARARGS, ""},
    {NULL}
};

/********************************************************************/

void initservicesystem(void)
{
    PyObject *m;

    PyProxyType.tp_getattro = (getattrofunc)PyObject_GenericGetAttr;
    PyProxyType.tp_setattro = (setattrofunc)PyObject_GenericSetAttr;
    if (PyType_Ready(&PyProxyType) < 0)
        return;

    m = Py_InitModule3("servicesystem", moduleMethods, "PyProxy module");
    if (m == NULL)
        return;

    Py_INCREF(&PyProxyType);
    PyModule_AddObject(m, "PyProxy", (PyObject *) &PyProxyType);

    /* Adding the module errors */
    UnknownServiceError = PyErr_NewException("servicesystem.UnknownServiceError", NULL, NULL);
    Py_INCREF(UnknownServiceError);
    PyModule_AddObject(m, "UnknownServiceError", UnknownServiceError);

    MarshallingError = PyErr_NewException("servicesystem.MarshallingError", NULL, NULL);
    Py_INCREF(MarshallingError);
    PyModule_AddObject(m, "MarshallingError", MarshallingError);
}

