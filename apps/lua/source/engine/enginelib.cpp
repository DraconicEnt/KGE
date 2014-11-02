#define lenginelib_cpp
#define LUA_LIB

#include <engine/CoreSingleton.hpp>

extern "C"
{
    #include <stdlib.h>

    #include "lua.h"

    #include "lauxlib.h"
    #include "lualib.h"

    static int engine_quit(lua_State *L)
    {
        Kiaro::Engine::CoreSingleton::getPointer()->kill();
        return 1;
    }

    static const luaL_Reg enginelib[] =
    {
      {"quit", engine_quit},
      {NULL, NULL}
    };

    LUAMOD_API int luaopen_engine(lua_State *L)
    {
        luaL_newlib(L, enginelib);
        return 1;
    }
}
