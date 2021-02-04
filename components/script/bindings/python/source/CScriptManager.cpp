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

#include <physfs.h>
#include <python3.9/Python.h>

#include <support/Console.hpp>
#include <script/modules/filesystem.hpp>
#include <script/CScriptManager.hpp>

namespace Kiaro
{
    namespace Script
    {
        CScriptManager::CScriptManager(void)
        {
            wchar_t* program = Py_DecodeLocale("KGE", NULL);
            Py_SetProgramName(program);

            // Once initialized, add module
            PyImport_AppendInittab("filesystem", &PyInit_filesystem);

            Py_Initialize();

            CONSOLE_INFO("Initialized Python 3 Interpreter");

            // Retrieve sys.path
            PyObject* sysModule = PyImport_ImportModule("sys");

            PyObject* pathAttribute = PyUnicode_FromString("path");
            PyObject* pythonSearchPath = PyObject_GetAttr(sysModule, pathAttribute);

            // Set search path
            unsigned int currentIndex = 0;
            char** physfsSearchPath = PHYSFS_getSearchPath();
            const char* currentPhysfsEntry = physfsSearchPath[currentIndex];
            while (currentPhysfsEntry)
            {
                PyObject* pythonPathEntry = PyUnicode_FromString(currentPhysfsEntry);

                CONSOLE_INFOF("Adding %s to search path for Python interpreter.", currentPhysfsEntry);
                ++currentIndex;
                currentPhysfsEntry = physfsSearchPath[currentIndex];

                PyList_Append(pythonSearchPath, pythonPathEntry);
            }
        }

        bool CScriptManager::loadMainScript(void)
        {
            CONSOLE_INFO("Loading Python Main Script");

            PyObject* mainModule = PyImport_ImportModule("main");
            if (!mainModule)
            {
                CONSOLE_ERROR("Could not import main.py from any search path!");
                return false;
            }
            return true;
        }
    } // End Namespace Script
} // End NameSpace Kiaro