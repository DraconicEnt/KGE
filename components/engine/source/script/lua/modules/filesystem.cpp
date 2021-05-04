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

#include <easylua.hpp>
#include <physfs.h>

#include <script/modules/filesystem.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Script
        {
            int filesystem_getDirectory(lua_State* lua)
            {
                const char* file = luaL_checkstring(lua, -1);
                lua_pop(lua, 1);

                const char* directory = PHYSFS_getRealDir(file);
                if (directory)
                {
                    lua_pushstring(lua, directory);
                }
                else
                {
                    lua_pushnil(lua);
                }
                return 1;
            }

            bool lua_initFileSystem(lua_State* lua)
            {
                // Register the public-facing Lua methods of our module.
                static const luaL_Reg filesystemFunctions[] = {
                    {"getLocation", filesystem_getDirectory},
                    {NULL, NULL}
                };
                luaL_newlib(lua, filesystemFunctions);
                lua_setglobal(lua, "filesystem");
                lua_pop(lua, 0);

                return true;
            }
        } // End Namespace Script
    } // End Namespace Engine
} // End NameSpace Kiaro
