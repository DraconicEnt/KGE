/**
 *  @file scenegraphlib.c
 */

#define lscenegraphlib_c
#define LUA_LIB

#include <stdlib.h>

#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"

#include <lualibs/scenegraphlib.h>

extern int lua_scenegraphinstance_drop(lua_State *L);
extern int lua_scenegraphinstance_setlightingenabled(lua_State *L);

static const luaL_Reg scenegraphinstancelib[] =
{
	{ "drop", lua_scenegraphinstance_drop },
	{ "setLightingEnabled", lua_scenegraphinstance_setlightingenabled },
	{ NULL, NULL }
};

// Scene Graph Lib
int lua_scenegraphlib_create(lua_State *L);

static const luaL_Reg scenegraphlib[] =
{
	{ "create", lua_scenegraphlib_create },
	{ NULL, NULL }
};

LUAMOD_API int luaopen_scenegraph(lua_State *L)
{
    // Register metatable for user data in registry
    luaL_newmetatable(L, "KGE.SceneGraph");
    // Bind the methods to our meta table
    luaL_setfuncs(L, scenegraphinstancelib, 0);

    // Perform method lookups on the meta table
    lua_pushvalue(L, -1);
    lua_setfield(L, -1, "__index");

    //lua_setglobal(L, "KGE.SceneGraph");

    // Initialize the main lib as normal
    luaL_newlib(L, scenegraphlib);

	return 1;
}

void *lua_checkSceneGraph(lua_State *l, int n)
{
    return *(void**)luaL_checkudata(l, n, "KGE.SceneGraph");
}
