/**
 *  @file easylua.hpp
 *  @version 1.1
 *  @date 2/17/2015
 *  @author <a href="https://github.com/Ragora">Robert MacGregor</a>
 */

#include <lua.hpp>

class EasyLua
{
    public:
        template <typename... parameters>
        static void pushParameters(lua_State *lua, int value, parameters... params)
        {
            lua_pushinteger(lua, value);
            pushParameters(lua, params...);
        }

        template <typename... parameters>
        static void pushParameters(lua_State *lua, float value, parameters... params)
        {
            lua_pushnumber(lua, value);
            pushParameters(lua, params...);
        }

        template <typename... parameters>
        static void pushParameters(lua_State *lua, const char *value, parameters... params)
        {
            lua_pushstring(lua, value);
            pushParameters(lua, params...);
        }

        static void pushParameters(lua_State *lua) { }

        template <typename... parameters>
        static void call(lua_State *lua, parameters... params)
        {
            pushParameters(lua, params...);
            lua_call(lua, sizeof...(params), 0);
        }
};
