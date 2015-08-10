#define lclientlib_c
#define LUA_LIB

#include <stdlib.h>

#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"

#include <lualibs/clientlib.h>

int lua_client_getport(lua_State *L);

static const luaL_Reg clientlib[] =
{
	{ "getport", lua_client_getport },
	{ NULL, NULL }
};

static int lua_client_toString (lua_State *L)
{
    lua_pushfstring(L, "Client: %p", lua_touserdata(L, 1));
    return 1;
}

static const luaL_Reg client_meta[] =
{
    { "__tostring", lua_client_toString },
    { NULL, NULL }
};

LUAMOD_API int luaopen_client(lua_State *L)
{
	//luaL_newmetatable(L, "client");
    //luaL_setfuncs(L, clientlib, -1);
    luaL_newlib(L, clientlib);

	return 1;
}
