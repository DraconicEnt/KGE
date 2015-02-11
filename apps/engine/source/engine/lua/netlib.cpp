#include <engine/CoreSingleton.hpp>
#include <network/OutgoingClientSingleton.hpp>

#include <stdlib.h>

#include <lua.hpp>

extern "C"
{
    int lua_net_connect(lua_State *L)
    {
        Kiaro::Engine::CoreSingleton *engineCore = Kiaro::Engine::CoreSingleton::getPointer();

        // The OutgoingClientSingleton isn't even initialized for a dedicated server
        if (engineCore->isDedicated())
            return luaL_error(L, "Method 'net.connect' is not available for dedicated servers!");
        else if (lua_gettop(L) == 0)
            return luaL_error(L, "No IP address specified for method 'net.connect'.");
        else if (!lua_isstring(L, -1))
            return luaL_error(L, "IP parameter for method 'net.connect' is not a string!");

        Kiaro::Network::OutgoingClientSingleton *outClient = Kiaro::Network::OutgoingClientSingleton::getPointer();

        outClient->disconnect();
        outClient->connect(lua_tostring(L, -1), 11595, 3000);

        return 1;
    }

    int lua_net_disconnect(lua_State *L)
    {
        Kiaro::Engine::CoreSingleton *engineCore = Kiaro::Engine::CoreSingleton::getPointer();

        if (engineCore->isDedicated())
            return luaL_error(L, "Method 'net.disconnect' is not available for dedicated servers!");

        Kiaro::Network::OutgoingClientSingleton *outClient = Kiaro::Network::OutgoingClientSingleton::getPointer();

        if (!outClient->getIsConnected())
            return luaL_error(L, "Method 'net.disconnect' called without an active connection to a server!");

        outClient->disconnect();

        return 1;
    }
}
