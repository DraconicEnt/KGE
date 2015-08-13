/**
 *  @file ServerSingleton.cpp
 *  @brief Include file defining the Server and related functions.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <net/IServer.hpp>

#include <net/CIncomingClient.hpp>

namespace Kiaro
{
    namespace Net
    {
        IServer::IServer(const Support::String& listenAddress, const Common::U16& listenPort, const Common::U32& maximumClientCount) :
        mLastPacketSender(NULL), mIsRunning(true), mInternalHost(NULL), mListenPort(listenPort), mListenAddress(listenAddress), mMaximumClientCount(maximumClientCount)
        {
           // Core::Logging::write(Core::Logging::MESSAGE_INFO, "IServer: Creating server on %s:%u with %u maximum clients ...", listenAddress.data(), listenPort, maximumClientCount);

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

            // Disconnect everyone
            for (Net::IServer::clientIterator it = this->clientsBegin(); it != this->clientsEnd(); it++)
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

        void IServer::update(const Common::F32& deltaTimeSeconds)
        {
            Net::IServer::networkUpdate(deltaTimeSeconds);

           // mEntityGroup->update(deltaTimeSeconds);
        }

        void IServer::globalSend(Net::IMessage* packet, const bool& reliable)
        {
            for (std::set<Net::CIncomingClient*>::iterator it = mConnectedClientSet.begin(); it != mConnectedClientSet.end(); it++)
                (*it)->send(packet, reliable);
        }

        void IServer::networkUpdate(const Common::F32& deltaTimeSeconds)
        {
            // Dispatch commit packets after we're done dispatching sim updates
           // Net::Messages::SimCommit commitPacket;
           // this->globalSend(&commitPacket, true);

            ENetEvent event;
            while (enet_host_service(mInternalHost, &event, 0) > 0)
            {
                switch(event.type)
                {
                    case ENET_EVENT_TYPE_CONNECT:
                    {
                        CIncomingClient* client = new CIncomingClient(event.peer, this);
                        event.peer->data = client;

                        mConnectedClientSet.insert(mConnectedClientSet.end(), client);
                        onClientConnected(client);

                        break;
                    }

                    case ENET_EVENT_TYPE_DISCONNECT:
                    {
                        CIncomingClient* disconnected = (CIncomingClient*)event.peer->data;
                        onClientDisconnected(disconnected);

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

                        CIncomingClient* sender = (CIncomingClient*)event.peer->data;
                        Support::CBitStream incomingStream(event.packet->data, event.packet->dataLength);

                        this->onReceivePacket(incomingStream, sender);
                        enet_packet_destroy(event.packet);

                        break;
                    }

                    case ENET_EVENT_TYPE_NONE:
                        break;
                }
            }
        }

        void IServer::onClientConnected(CIncomingClient* client)
        {
            // Can we accept this guy?
            if (mConnectedClientSet.size() >= mMaximumClientCount)
            {
                client->disconnect("Server is full. ");
                return;
            }

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

        void IServer::onClientDisconnected(CIncomingClient* client)
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

        void IServer::onReceivePacket(Support::CBitStream& incomingStream, CIncomingClient* sender)
        {
            mLastPacketSender = sender;

//            Net::IMessage basePacket;
//            basePacket.extractFrom(incomingStream);

            /*
            switch (basePacket.getType())
            {
                case Net::Messages::MESSAGE_HANDSHAKE:
                {
                    Net::Messages::HandShake receivedHandshake;
                    receivedHandshake.extractFrom(incomingStream);

                  //  Core::Logging::write(Core::Logging::MESSAGE_INFO, "IServer: Client version is %u.%u.%u.%u.", receivedHandshake.mVersionMajor,
                  //  receivedHandshake.mVersionMinor, receivedHandshake.mVersionRevision, receivedHandshake.mVersionBuild);

                    Net::Messages::HandShake handShake;

                    sender->send(&handShake, true);
                    break;
                }
            }
            */

            mLastPacketSender = NULL;
        }

        CIncomingClient* IServer::getLastPacketSender(void)
        {
            CIncomingClient* result = mLastPacketSender;
            mLastPacketSender = NULL;

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
