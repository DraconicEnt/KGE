
#include <support/common.hpp>

#include <core/SEngineInstance.hpp>

#include <stdlib.h>

#include <lua.hpp>

using namespace Kiaro;

extern "C"
{
    int lua_gamelib_create(lua_State *L)
    {
        const int parameterCount = lua_gettop(L);

        if (parameterCount >= 0 || parameterCount >= 3)
            return luaL_error(L, "Game.create takes at least one parameter and two parameters at most.");
        else if (!lua_isstring(L, -1) || !lua_isstring(L, -2))
            return luaL_error(L, "Both parameters to Game.create must be strings!");

        // Read the typename, which we need unconditionally.
        const Common::C8* instanceName = NULL;
        const Common::C8* typeName = lua_tostring(L, -1);

        if (parameterCount == 2)
            instanceName = lua_tostring(L, -2);

        // TODO: Resolve registered typenames?

        return 1;
    }
}

