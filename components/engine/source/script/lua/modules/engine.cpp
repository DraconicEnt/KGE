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

#include <core/SEngineInstance.hpp>
#include <script/modules/engine.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Script
        {
            int engine_kill(lua_State* lua)
            {
                Kiaro::Engine::Core::SEngineInstance* engine = Kiaro::Engine::Core::SEngineInstance::getInstance();
                engine->kill();
                return 0;
            }

            bool lua_initEngine(lua_State* lua)
            {
                // Register the public-facing Lua methods of our module.
                static const luaL_Reg engineFunctions[] = {
                    {"kill", engine_kill},
                    {NULL, NULL}
                };
                luaL_newlib(lua, engineFunctions);
                lua_setglobal(lua, "engine");
                lua_pop(lua, 0);

                return true;
            }
        } // End Namespace Script
    } // End Namespace Engine
} // End NameSpace Kiaro
