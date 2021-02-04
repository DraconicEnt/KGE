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

extern "C"
{
    #include <lua5.2/lualib.h>
    #include <lua5.2/lauxlib.h>
    #include <lua5.2/lua.h>
}

namespace Kiaro
{
    namespace Engine
    {
        namespace Script
        {
            bool lua_initFileSystem(lua_State* lua);
        } // End Namespace Script
    } // End Namespace Engine
} // End NameSpace Kiaro
