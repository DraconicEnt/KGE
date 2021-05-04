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

#include <irrlicht.h>
#include <easylua.hpp>
#include <physfs.h>

#include <video/CGraphicsWindow.hpp>
#include <script/modules/video.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Script
        {
            int video_createWindow(lua_State* lua)
            {
                /*
                    Expect a table that looks like this:

                    {
                        title = "title",
                        fullscreen = true/false,
                        resolution = {
                            640,
                            480
                        }
                    }
                */

                luaL_checktype(lua, -1, LUA_TTABLE);

                lua_getfield(lua, -1, "resolution");
                lua_getfield(lua, -2, "title");
                lua_getfield(lua, -3, "fullscreen");

                bool fullscreen = lua_toboolean(lua, -1) == 1;
                const char* title = luaL_checkstring(lua, -2);

                // Pop title and fullscreen, the values are loaded already
                lua_pop(lua, 2);

                // Now process the resolution table
                luaL_checktype(lua, -1, LUA_TTABLE);
                lua_rawgeti(lua, -1, 1);

                int width = luaL_checknumber(lua, -1);
                lua_rawgeti(lua, -2, 1);
                int height = luaL_checknumber(lua, -1);

                // Create a window instance with this data
                Kiaro::Engine::Video::CGraphicsWindow::WindowParameters parameters;
                parameters.mTitle = title;
                parameters.mFullscreen = fullscreen;
                parameters.mResolution = Support::Dimension2DU(width, height);

                Kiaro::Engine::Video::CGraphicsWindow* newWindow = new Kiaro::Engine::Video::CGraphicsWindow(parameters);
                newWindow->initialize();

                lua_pushnil(lua);

                return 1;
            }

            bool lua_initVideo(lua_State* lua)
            {
                // Register the public-facing Lua methods of our module.
                static const luaL_Reg videoFunctions[] = {
                    {"createWindow", video_createWindow},
                    {NULL, NULL}
                };
                luaL_newlib(lua, videoFunctions);

                lua_pushstring(lua, "driver");
                lua_newtable(lua);

                // Set driver fields
                lua_pushstring(lua, "opengl");
                lua_pushnumber(lua, irr::video::EDT_OPENGL);
                lua_rawset(lua, -3);
                lua_pushstring(lua, "software");
                lua_pushnumber(lua, irr::video::EDT_SOFTWARE);
                lua_rawset(lua, -3);
                lua_pushstring(lua, "burnings");
                lua_pushnumber(lua, irr::video::EDT_BURNINGSVIDEO);
                lua_rawset(lua, -3);
                lua_pushstring(lua, "d3d9");
                lua_pushnumber(lua, irr::video::EDT_DIRECT3D9);
                lua_rawset(lua, -3);
                lua_pushstring(lua, "d3d8");
                lua_pushnumber(lua, irr::video::EDT_DIRECT3D8);
                lua_rawset(lua, -3);

                // Set 'driver' field of 'video' table
                lua_rawset(lua, -3);
                lua_setglobal(lua, "video");

                return true;
            }
        } // End Namespace Script
    } // End Namespace Engine
} // End NameSpace Kiaro
