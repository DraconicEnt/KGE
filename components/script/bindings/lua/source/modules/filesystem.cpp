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

#include <script/modules/filesystem.hpp>

namespace Kiaro
{
    namespace Script
    {
        int luapi_wait_msec (lua_State *L)
        {
            int msec = (int) luaL_checknumber (L, -1);
            lua_pop (L, 1);
            printf("YEET\n");
            //usleep (msec * 1000);
            return 0;
        }

        bool lua_initFileSystem(lua_State* lua)
        {
            // Register the public-facing Lua methods of our module.
            static const luaL_Reg fns[] = {
              {"balls", luapi_wait_msec},
              {NULL, NULL}
            };
            luaL_newlib(lua, fns);

        //    lua_pushstring(lua, "test");
            //lua_setglobal(lua, "test");
            //luaL_register(lua, "apidemo", fns);


                   // lua_newtable(lua);
            //lua_pushcfunction (lua, luapi_wait_msec);
           // lua_setfield (lua, -2, "wait_msec");
            //lua_setglobal(lua, "test");

            return true;
        }
    } // End Namespace Script
} // End NameSpace Kiaro
