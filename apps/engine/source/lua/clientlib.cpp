#include <core/SEngineInstance.hpp>
#include <net/IOutgoingClient.hpp>
#include <net/CIncomingClient.hpp>

#include <stdlib.h>

#include <lua.hpp>
#include <lualibs/clientlib.h>

extern "C"
{
    int lua_client_getport(lua_State *L)
    {
        Client *luaClient = (Client *)lua_touserdata(L, 1);

        Kiaro::Net::CIncomingClient *client = reinterpret_cast<Kiaro::Net::CIncomingClient *>(luaClient->mPointer);
        lua_pushnumber(L, client->getPort());

        return 1;
    }
}
