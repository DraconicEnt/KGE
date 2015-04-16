#ifndef _INCLUDE_LUA_CLIENTLIB_HPP
#define _INCLUDE_LUA_CLIENTLIB_HPP

typedef struct
{
    void *mPointer;
} Client;

static Client *lua_pushclient(lua_State *L, void *client)
{
    Client *newClient = (Client *)lua_newuserdata(L, 4);
    newClient->mPointer = client;

    //luaL_getmetatable(L, "client");
    //lua_setmetatable(L, -2);

    return newClient;
}

#endif // _INCLUDE_LUA_CLIENTLIB_HPP
