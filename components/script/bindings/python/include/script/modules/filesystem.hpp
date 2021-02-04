/**
 *  @file SRenderer.hpp
 *  @brief Include file declaring the SRenderer singleton class and its methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2021 Draconic Entity
 */


#include <python3.9/Python.h>

namespace Kiaro
{
    namespace Script
    {
        static PyObject* filesystem_getPath(PyObject *self, PyObject *args, PyObject *keywds)
        {
            Py_RETURN_NONE;
        }

        static PyMethodDef filesystem_methods[] = {
            {"get_path", (PyCFunction)(void(*)(void))filesystem_getPath, METH_VARARGS | METH_KEYWORDS, "Retrieve the real path to an item."},
            {NULL, NULL, 0, NULL}   /* sentinel */
        };

        static struct PyModuleDef filesystemModule = {
            PyModuleDef_HEAD_INIT,
            "filesystem",
            NULL,
            -1,
            filesystem_methods
        };

        static PyObject* PyInit_filesystem(void)
        {
            printf("wut\n");
            return PyModule_Create(&filesystemModule);
        }
    } // End Namespace Script
} // End NameSpace Kiaro