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
#include "pymemoryproxy.h"
#include "model/ModelControl.h"
#include "plugins/processor/MemoryInterface.h"
#include "stdlib.h"

typedef struct {
    PyObject_HEAD
    /* Type-specific fields go here. */
    MemoryByteInterface* byte_interface;
} modelcontrol_MemoryByteInterfaceObject;

static PyObject*
MemoryByteInterfaceObject_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    modelcontrol_MemoryByteInterfaceObject *self;

    self = (modelcontrol_MemoryByteInterfaceObject*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->byte_interface = 0;

        PyObject* o;
        int ok = PyArg_ParseTuple(args, "O", &o);
        if (PyInt_Check(o))
        {
            long interface_index = PyInt_AsLong(o);
            self->byte_interface = ModelControl::getInstance()->getByteInterfaceByIndex(interface_index);
        }
    }

    return (PyObject*)self;
}

static void MemoryByteInterfaceObject_dealloc(modelcontrol_MemoryByteInterfaceObject* self)
{
    delete self->byte_interface;
    self->ob_type->tp_free((PyObject*)self);
}

static PyObject* MemoryByteInterfaceObject_getbyte(modelcontrol_MemoryByteInterfaceObject* self, PyObject *args)
{
    PyObject *result;
    int address;

    int ok = PyArg_ParseTuple(args, "i", &address);
    if (!self->byte_interface)
    {
        Py_RETURN_NONE;
    } else {
        unsigned char value;
        bool success = self->byte_interface->getByte(address, value);
        if (!success)
        {
            Py_RETURN_NONE;
        }
        result = Py_BuildValue("i", value);
    }
    return result;
}

static PyObject* MemoryByteInterfaceObject_setbyte(modelcontrol_MemoryByteInterfaceObject* self, PyObject *args)
{
    int address, value;
    int ok = PyArg_ParseTuple(args, "ii", &address, &value);
    if (self->byte_interface)
    {
        bool success = self->byte_interface->setByte(address, value);
    }

    Py_RETURN_NONE;
}

static PyMethodDef MemoryByteInterfaceObject_methods[] = {
    {"get_byte", (PyCFunction)MemoryByteInterfaceObject_getbyte, METH_VARARGS,
        "return a byte from this memory interface" },
    {"set_byte", (PyCFunction)MemoryByteInterfaceObject_setbyte, METH_VARARGS,
        "set a byte on this memory interface" },
    {NULL}
};

static PyTypeObject modelcontrol_MemoryByteInterfaceType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "modelcontrol.MemoryByteInterface", /*tp_name*/
    sizeof(modelcontrol_MemoryByteInterfaceObject), /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)MemoryByteInterfaceObject_dealloc, /*tp_dealloc*/
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
    "Memory Interface class to control content of the processors memory", /* tp_doc */
    0,                         /*tp_traverse*/
    0,                         /*tp_clear*/
    0,                         /*tp_richcompare*/
    0,                         /*tp_weaklistoffset*/
    0,                         /*tp_iter*/
    0,                         /*tp_iternext*/
    MemoryByteInterfaceObject_methods, /*tp_methods*/
    0,                         /*tp_members*/
    0,                         /*tp_getset*/
    0,                         /*tp_base*/
    0,                         /*tp_dict*/
    0,                         /*tp_descr_get*/
    0,                         /*tp_descr_set*/
    0,                         /*tp_dictoffset*/
    0,                         /*tp_init*/
    0,                         /*tp_alloc*/
    MemoryByteInterfaceObject_new, /*tp_new*/
    0,                         /*tp_free*/
    0,                         /*tp_is_gc*/
    0,                         /*tp_bases*/
    0,                         /*tp_mro*/
    0,                         /*tp_cache*/
    0,                         /*tp_subclasses*/
    0,                         /*tp_weaklist*/
};

void add_memoryinterface_to_module(PyObject* module) 
{
    if (PyType_Ready(&modelcontrol_MemoryByteInterfaceType) < 0)
    {
        printf("The MemoryByteInterface type is not ready, ooh my goooooood!!!\n");
        return;
    }

    Py_INCREF(&modelcontrol_MemoryByteInterfaceType);
    PyModule_AddObject(module, "MemoryByteInterface", (PyObject *)&modelcontrol_MemoryByteInterfaceType);
}

