#include <engine/SEngineInstance.hpp>
#include <network/SClient.hpp>
#include <network/CClient.hpp>

#include <stdlib.h>

#include <lua.hpp>
#include <lualibs/clientlib.h>

extern "C"
{
    int lua_client_getport(lua_State *L)
    {
        Client *luaClient = (Client *)lua_touserdata(L, 1);

        Kiaro::Network::CClient *client = reinterpret_cast<Kiaro::Network::CClient *>(luaClient->mPointer);
        lua_pushnumber(L, client->getPort());

        return 1;
    }
}
