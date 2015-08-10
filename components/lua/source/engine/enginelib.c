#define lenginelib_c
#define LUA_LIB

#include <stdlib.h>

#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"

int lua_engine_quit(lua_State *L);

static const luaL_Reg enginelib[] =
{
	{ "quit", lua_engine_quit },
	{ NULL, NULL }
};

LUAMOD_API int luaopen_engine(lua_State *L)
{
	luaL_newlib(L, enginelib);
	return 1;
}