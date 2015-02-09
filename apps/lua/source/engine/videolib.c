#define lvideolib_c
#define LUA_LIB

#include <stdlib.h>

#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"

extern int lua_video_setresolution(lua_State *L);

static const luaL_Reg videolib[] =
{
	{ "setResolution", lua_video_setresolution },
	{ NULL, NULL }
};

LUAMOD_API int luaopen_video(lua_State *L)
{
	luaL_newlib(L, videolib);
	return 1;
}
