#include <iostream>

#include "DDImage/plugins.h"
#include <Python.h>


static PyObject*
is_verbose(PyObject *self, PyObject *args)
{
    return PyBool_FromLong(DD::Image::plugin_verbose);
}


PyMethodDef methods[] = {
    {"is_verbose", is_verbose, METH_VARARGS,
     "Returns verbosity of Nuke"},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC 
initnuke_verbosity()
{
    (void) Py_InitModule("nuke_verbosity", methods);   
}
