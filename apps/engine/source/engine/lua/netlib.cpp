#include <engine/SEngineInstance.hpp>
#include <network/SClient.hpp>

#include <stdlib.h>

#include <lua.hpp>

extern "C"
{
    int lua_net_connect(lua_State *L)
    {
        Kiaro::Engine::SEngineInstance *engineCore = Kiaro::Engine::SEngineInstance::getPointer();

        // The SClient isn't even initialized for a dedicated server
        if (engineCore->isDedicated())
            return luaL_error(L, "Method 'net.connect' is not available for dedicated servers!");
        else if (lua_gettop(L) == 0)
            return luaL_error(L, "No IP address specified for method 'net.connect'.");
        else if (!lua_isstring(L, -1))
            return luaL_error(L, "IP parameter for method 'net.connect' is not a string!");

        Kiaro::Network::SClient *outClient = Kiaro::Network::SClient::getPointer();

        outClient->disconnect();

        Kiaro::Common::U16 port = 11595;
        Kiaro::Common::String address = "127.0.0.1";

        if (lua_gettop(L) == 1)
            address = lua_tostring(L, -1);
        else
        {
            port = lua_tointeger(L, -1);
            address = lua_tostring(L, -2);
        }

        outClient->connect(address, port, 3000);

        return 1;
    }

    int lua_net_disconnect(lua_State *L)
    {
        Kiaro::Engine::SEngineInstance *engineCore = Kiaro::Engine::SEngineInstance::getPointer();

        if (engineCore->isDedicated())
            return luaL_error(L, "Method 'net.disconnect' is not available for dedicated servers!");

        Kiaro::Network::SClient *outClient = Kiaro::Network::SClient::getPointer();

        if (!outClient->getIsConnected())
            return luaL_error(L, "Method 'net.disconnect' called without an active connection to a server!");

        outClient->disconnect();

        return 1;
    }
}
