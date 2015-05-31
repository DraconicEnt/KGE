#include <core/SEngineInstance.hpp>
#include <net/SClient.hpp>
#include <net/CClient.hpp>

#include <stdlib.h>

#include <lua.hpp>
#include <lualibs/clientlib.h>

extern "C"
{
    int lua_client_getport(lua_State *L)
    {
        Client *luaClient = (Client *)lua_touserdata(L, 1);

        Kiaro::Net::CClient *client = reinterpret_cast<Kiaro::Net::CClient *>(luaClient->mPointer);
        lua_pushnumber(L, client->getPort());

        return 1;
    }
}
