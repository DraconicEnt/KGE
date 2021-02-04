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

#include <support/Console.hpp>
#include <script/modules/filesystem.hpp>
#include <script/CScriptManager.hpp>

namespace Kiaro
{
    namespace Script
    {
        CScriptManager::CScriptManager(void)
        {
            this->luaState = luaL_newstate();
            luaL_openlibs(this->luaState);

            // Register modules
            lua_initFileSystem(this->luaState);

            CONSOLE_INFO("Initialized new Lua virtual machine");
        }

        bool CScriptManager::loadMainScript(void)
        {
            // Search for main.lua
            const char* mainFile = PHYSFS_getRealDir("main.lua");
            if (!mainFile)
            {
                return false;
            }

            CONSOLE_INFO("Loading main.lua ...");
            PHYSFS_File* handle = PHYSFS_openRead("main.lua");
            if (handle)
            {
                PHYSFS_sint64 fileLength = PHYSFS_fileLength(handle);

                char codeBuffer[fileLength];
                PHYSFS_read(handle, codeBuffer, fileLength, 1);

                if (luaL_loadstring(this->luaState, codeBuffer) == LUA_OK) 
                {
                    if (lua_pcall(this->luaState, 0, 1, 0) == LUA_OK) 
                    {
                        lua_pop(this->luaState, lua_gettop(this->luaState));
                    }
                }
            } 
            else
            {
                CONSOLE_ERROR("Could not find main.lua!");
                return false;
            }

            return true;
        }
    } // End Namespace Script
} // End NameSpace Kiaro