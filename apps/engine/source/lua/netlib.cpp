#include <core/SEngineInstance.hpp>
#include <net/IOutgoingClient.hpp>

#include <stdlib.h>

#include <lua.hpp>

using namespace Kiaro;

extern "C"
{
    int lua_net_connect(lua_State *L)
    {
        Core::SEngineInstance *engineCore = Core::SEngineInstance::getPointer();

        // The SClient isn't even initialized for a dedicated server
        if (engineCore->isDedicated())
            return luaL_error(L, "Method 'net.connect' is not available for dedicated servers!");
        else if (lua_gettop(L) == 0)
            return luaL_error(L, "No IP address specified for method 'net.connect'.");
        else if (!lua_isstring(L, -1))
            return luaL_error(L, "IP parameter for method 'net.connect' is not a string!");

/*
        Net::SClient *outClient = Net::SClient::getPointer();

        outClient->disconnect();

        Common::U16 port = 11595;
        Support::String address = "127.0.0.1";

        if (lua_gettop(L) == 1)
            address = lua_tostring(L, -1);
        else
        {
            port = lua_tointeger(L, -1);
            address = lua_tostring(L, -2);
        }

        outClient->connect(address, port, 3000);
*/

        return 1;
    }

    int lua_net_disconnect(lua_State *L)
    {
    /*
        Core::SEngineInstance *engineCore = Core::SEngineInstance::getPointer();

        if (engineCore->isDedicated())
            return luaL_error(L, "Method 'net.disconnect' is not available for dedicated servers!");

        Net::SClient *outClient = Net::SClient::getPointer();

        if (!outClient->getIsConnected())
            return luaL_error(L, "Method 'net.disconnect' called without an active connection to a server!");

        outClient->disconnect();
*/

        return 1;
    }
}
