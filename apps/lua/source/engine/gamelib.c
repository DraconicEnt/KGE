 /**
 *  @file gamelib.c
 */

#define lgamelib_c
#define LUA_LIB

#include <stdlib.h>

#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"

#include <lualibs/gamelib.h>

extern int lua_gamelib_create(lua_State *L);

static const luaL_Reg gamelib[] =
{
	{ "create", lua_gamelib_create },
	{ NULL, NULL }
};

LUAMOD_API int luaopen_game(lua_State *L)
{
    // Initialize the main lib as normal
    luaL_newlib(L, gamelib);

	return 1;
}
