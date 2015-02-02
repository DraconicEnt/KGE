#define lnetlib_c
#define LUA_LIB

#include <stdlib.h>

#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"

extern int lua_net_connect(lua_State *L);
extern int lua_net_disconnect(lua_State *L);

static const luaL_Reg netlib[] =
{
	{ "connect", lua_net_connect },
	{ "disconnect", lua_net_disconnect },
	{ NULL, NULL }
};

LUAMOD_API int luaopen_net(lua_State *L)
{
	luaL_newlib(L, netlib);
	return 1;
}
