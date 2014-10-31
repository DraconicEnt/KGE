#define lnetlib_cpp
#define LUA_LIB

#include <engine/CoreSingleton.hpp>

extern "C"
{
    #include <stdlib.h>

    #include "lua.h"

    #include "lauxlib.h"
    #include "lualib.h"

    /*
    static int math_log (lua_State *L) {
      lua_Number x = luaL_checknumber(L, 1);
      lua_Number res;
      if (lua_isnoneornil(L, 2))
        res = l_mathop(log)(x);
      else {
        lua_Number base = luaL_checknumber(L, 2);
        if (base == (lua_Number)10.0) res = l_mathop(log10)(x);
        else res = l_mathop(log)(x)/l_mathop(log)(base);
      }
      lua_pushnumber(L, res);
      return 1;
    }
    */

    static int net_connect(lua_State *L)
    {
        Kiaro::Engine::CoreSingleton *engineCore = Kiaro::Engine::CoreSingleton::getPointer();
        return 1;
    }

    static const luaL_Reg netlib[] =
    {
      {"connect", net_connect},
      {NULL, NULL}
    };

    LUAMOD_API int luaopen_net(lua_State *L)
    {
        luaL_newlib(L, netlib);
        return 1;
    }
}
