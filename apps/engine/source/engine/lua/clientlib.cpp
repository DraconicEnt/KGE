#include <engine/CoreSingleton.hpp>
#include <network/OutgoingClientSingleton.hpp>
#include <network/IncomingClient.hpp>

#include <stdlib.h>

#include <lua.hpp>
#include <lualibs/clientlib.hpp>

extern "C"
{
    int lua_client_getport(lua_State *L)
    {
        Client *luaClient = (Client *)lua_touserdata(L, 1);

        Kiaro::Network::IncomingClient *client = reinterpret_cast<Kiaro::Network::IncomingClient *>(luaClient->mPointer);
        lua_pushnumber(L, client->getPort());

        return 1;
    }
}
