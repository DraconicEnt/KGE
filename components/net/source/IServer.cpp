/**
 *  @file ServerSingleton.cpp
 *  @brief Include file defining the Server and related functions.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2015 Draconic Entity
 */

#include <support/Console.hpp>

#include <net/IServer.hpp>
#include <net/IIncomingClient.hpp>

namespace Kiaro
{
    namespace Net
    {
        IServer::IServer(const Support::String& listenAddress, const Common::U16& listenPort, const Common::U32& maximumClientCount) :
        mLastPacketSender(NULL), mIsRunning(true), mInternalHost(NULL), mListenPort(listenPort), mListenAddress(listenAddress), mMaximumClientCount(maximumClientCount)
        {
            Support::Console::writef(Support::Console::MESSAGE_INFO, "IServer: Creating server on %s:%u with %u maximum clients ...", listenAddress.data(), listenPort, maximumClientCount);

            ENetAddress enetAddress;
            enetAddress.port = listenPort;
            enet_address_set_host(&enetAddress, listenAddress.c_str());

            // FIXME (Robert MacGregor#9): Reliable dispatch of server full messages?
            mInternalHost = enet_host_create(&enetAddress, maximumClientCount + 1, 2, 0, 0);
            if (!mInternalHost)
            {
                mIsRunning = false;

                throw std::runtime_error("IServer: Failed to create ENet host!");
            }

            // Setup some more of the Lua implements
            /*
            lua_State* luaState = Core::SEngineInstance::getPointer()->getLuaState();

            lua_getglobal(luaState, "GameServer");
            lua_pushstring(luaState, "listenPort");
            lua_pushnumber(luaState, listenPort);
            lua_settable(luaState, -3);

            lua_pushstring(luaState, "listenAddress");
            lua_pushstring(luaState, listenAddress.c_str());
            lua_settable(luaState, -3);

            // The table "GameServer" is left so we chop it off
            lua_pop(luaState, -1);
            */

            // Create the map division
            //Support::CMapDivision::Get(12);

           // mEntityGroup = Game::SGameWorld::getPointer();

            // Create an init a gamemode
        }

        IServer::~IServer(void)
        {
            // Call the server shutdown sequence in Lua
            /*
            lua_State* lua = Core::SEngineInstance::getPointer()->getLuaState();
            lua_getglobal(lua, "GameServer");
            lua_getfield(lua, -1, "onShutdown");
            lua_call(lua, 0, 0);
            */

            Support::Console::write(Support::Console::MESSAGE_INFO, "IServer: Deinitializing game server ...");

            // Disconnect everyone
            for (auto it = this->clientsBegin(); it != this->clientsEnd(); it++)
                (*it)->disconnect("Server Shutdown");

           // Game::SGameWorld::destroy();

            if (mInternalHost)
            {
                enet_host_flush(mInternalHost); // Make sure we dispatch disconnects
                enet_host_destroy(mInternalHost);
                mInternalHost = NULL;
            }

            mIsRunning = false;

           // Support::CMapDivision::Destroy();
        }

        void IServer::globalSend(IMessage* packet, const bool& reliable)
        {
            for (auto it = mConnectedClientSet.begin(); it != mConnectedClientSet.end(); it++)
                (*it)->send(packet, reliable);
        }

        void IServer::update(const Common::F32& deltaTimeSeconds)
        {
            // TODO (Robert MacGregor#9): Dispatch commit packets after we're done dispatching sim updates
           // Net::Messages::SimCommit commitPacket;
           // this->globalSend(&commitPacket, true);

            ENetEvent event;
            while (enet_host_service(mInternalHost, &event, 0) > 0)
                switch(event.type)
                {
                    case ENET_EVENT_TYPE_CONNECT:
                    {
                        Support::Console::write(Support::Console::MESSAGE_INFO, "IServer: Received client connect challenge.");

                        IIncomingClient* client = this->onReceiveClientChallenge(event.peer);
                        event.peer->data = client;

                        mPendingClientSet.insert(mPendingClientSet.end(), client);

                        break;
                    }

                    case ENET_EVENT_TYPE_DISCONNECT:
                    {
                        Support::Console::write(Support::Console::MESSAGE_INFO, "IServer: Received client disconnect.");

                        IIncomingClient* disconnected = reinterpret_cast<IIncomingClient*>(event.peer->data);
                        onClientDisconnected(disconnected);
                        disconnected->mIsConnected = false;

                        // TODO (Robert MacGregor#9): Delete from the correct set
                        mConnectedClientSet.erase(disconnected);

                        delete disconnected;

                        break;
                    }

                    case ENET_EVENT_TYPE_RECEIVE:
                    {
                        if (!event.peer->data)
                        {
                            enet_packet_destroy(event.packet);
                            throw std::runtime_error("IServer: Invalid ENet peer data on packet receive!");
                        }

                        IIncomingClient* sender = reinterpret_cast<IIncomingClient*>(event.peer->data);

                        mLastPacketSender = sender;

                        Support::CBitStream incomingStream(event.packet->data, event.packet->dataLength);
                        this->processPacket(incomingStream, sender);
                        enet_packet_destroy(event.packet);

                        mLastPacketSender = nullptr;

                        break;
                    }

                    case ENET_EVENT_TYPE_NONE:
                        break;
                }
        }

        void IServer::processPacket(Support::CBitStream& incomingStream, Net::IIncomingClient* sender)
        {
            assert(!incomingStream.isEmpty());

            this->onReceivePacket(incomingStream, sender);
        }

        void IServer::onClientConnected(IIncomingClient* client)
        {

            // Call the Lua callback
            /*
            lua_State *lua = Core::SEngineInstance::getPointer()->getLuaState();
            lua_getglobal(lua, "GameServer");
            lua_getfield(lua, -1, "onClientConnected");

            lua_pushclient(lua, client);
            lua_call(lua, 1, 0);
            */

          //  Core::SEventManager::get()->mOnClientConnectedEvent.invoke(client);
            //Core::Logging::write(Core::Logging::MESSAGE_INFO, "IServer: Received remote connection from %s:%u.", client->getStringIPAddress().data(), client->getPort());

            // FIXME (Robert MacGregor#9): Net some objects across their entire net sequence; perform only on proper client auth
           // Net::Messages::Scope scope;
          //  const Game::Entities::IEntity* const* activeEntities = Game::SGameWorld::getPointer()->getEntities();

           // for (Common::U32 iteration = 0; iteration < 4096; iteration++)
            //    if (activeEntities[iteration] && activeEntities[iteration]->getHintMask() & Game::Entities::NO_SCOPING)
            //        scope.add(activeEntities[iteration]);

            //client->send(&scope, true);
        }

        void IServer::onClientDisconnected(IIncomingClient* client)
        {
            /*
            lua_State* lua = Core::SEngineInstance::getPointer()->getLuaState();
            lua_getglobal(lua, "GameServer");
            lua_getfield(lua, -1, "onClientDisconnected");

            lua_pushclient(lua, client);
            lua_call(lua, 1, 0);
            */

         //   Core::SEventManager::get()->mOnClientDisconnectedEvent.invoke(client);
            //Core::Logging::write(Core::Logging::MESSAGE_INFO, "IServer: Received disconnection from %s:%u.", client->getStringIPAddress().data(), client->getPort());
        }

        IIncomingClient* IServer::getLastPacketSender(void)
        {
            IIncomingClient* result = mLastPacketSender;
            mLastPacketSender = nullptr;

            return result;
        }

        void IServer::stop(void) { mIsRunning = false; }

        Common::U32 IServer::getClientCount(void)
        {
            return mConnectedClientSet.size();
        }

        void IServer::dispatch(void)
        {
            if (mIsRunning)
                enet_host_flush(mInternalHost);
        }
    } // End Namespace Game
} // End Namespace Kiaro
