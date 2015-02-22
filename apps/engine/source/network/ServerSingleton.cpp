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

#include <engine/EntityGroupingSingleton.hpp>

#include <support/EventManagerSingleton.hpp>

#include <game/messages/HandShake.hpp>
#include <game/messages/SimCommit.hpp>
#include <game/messages/Disconnect.hpp>
#include <network/ServerSingleton.hpp>

#include <network/IncomingClient.hpp>

#include <game/entities/Sky.hpp>

#include <support/MapDivision.hpp>

#include <game/GamemodeBase.hpp>

#include <lualibs/clientlib.hpp>

namespace Kiaro
{
    namespace Network
    {
        ServerSingleton *ServerSingleton_Instance = NULL;

        ServerSingleton *ServerSingleton::getPointer(const Kiaro::Common::String &listenAddress, const Kiaro::Common::U16 &listenPort, const Kiaro::Common::U32 &maximumClientCount)
        {
            if (!ServerSingleton_Instance)
                ServerSingleton_Instance = new ServerSingleton(listenAddress, listenPort, maximumClientCount);

            return ServerSingleton_Instance;
        }

        void ServerSingleton::destroy(void)
        {
            if (ServerSingleton_Instance)
                delete ServerSingleton_Instance;

            ServerSingleton_Instance = NULL;
        }

        ServerSingleton::ServerSingleton(const Kiaro::Common::String &listenAddress, const Kiaro::Common::U16 &listenPort, const Kiaro::Common::U32 &maximumClientCount) :
        mLastPacketSender(NULL), mIsRunning(true), mInternalHost(NULL), mListenPort(listenPort), mCurrentGamemode(NULL), mListenAddress(listenAddress), mMaximumClientCount(maximumClientCount)
        {
            ENetAddress enetAddress;
            enetAddress.port = listenPort;
            enet_address_set_host(&enetAddress, listenAddress.c_str());

            // FIXME (Robert MacGregor#9): Reliable dispatch of server full messages?
            mInternalHost = enet_host_create(&enetAddress, maximumClientCount + 1, 2, 0, 0);
            if (!mInternalHost)
            {
                mIsRunning = false;

                throw std::runtime_error("ServerSingleton: Failed to create ENet host!");
            }

            // Setup some more of the Lua implements
            lua_State *luaState = Kiaro::Engine::CoreSingleton::getPointer()->getLuaState();

            lua_getglobal(luaState, "GameServer");
            lua_pushstring(luaState, "listenPort");
            lua_pushnumber(luaState, listenPort);
            lua_settable(luaState, -3);

            lua_pushstring(luaState, "listenAddress");
            lua_pushstring(luaState, listenAddress.c_str());
            lua_settable(luaState, -3);

            // Create the map division
            Kiaro::Support::MapDivision::Get(12);

            mEntityGroup = Kiaro::Engine::EntityGroupingSingleton::getPointer();

            // All entities register themselves with the Entity grouping singleton
            new Kiaro::Game::Entities::Sky();

            // Create an init a gamemode
        }

        ServerSingleton::~ServerSingleton(void)
        {
            // Call the server shutdown sequence in Lua
            lua_State *lua = Kiaro::Engine::CoreSingleton::getPointer()->getLuaState();
            lua_getglobal(lua, "GameServer");
            lua_getfield(lua, -1, "onShutdown");

            lua_call(lua, 0, 0);

            // Disconnect everyone
            for (Kiaro::Network::ServerSingleton::clientIterator it = this->clientsBegin(); it != this->clientsEnd(); it++)
                (*it)->disconnect("Server Shutdown");

            Kiaro::Engine::EntityGroupingSingleton::destroy();

            if (mInternalHost)
            {
                enet_host_flush(mInternalHost); // Make sure we dispatch disconnects
                enet_host_destroy(mInternalHost);
                mInternalHost = NULL;
            }

            mIsRunning = false;

            Kiaro::Support::MapDivision::Destroy();
        }

        void ServerSingleton::update(const Kiaro::Common::F32 &deltaTimeSeconds)
        {
            Kiaro::Network::ServerSingleton::networkUpdate(deltaTimeSeconds);

            mEntityGroup->update(deltaTimeSeconds);
        }

        void ServerSingleton::globalSend(Kiaro::Network::MessageBase *packet, const bool &reliable)
        {
            for (std::set<Kiaro::Network::IncomingClient*>::iterator it = mConnectedClientSet.begin(); it != mConnectedClientSet.end(); it++)
                (*it)->send(packet, reliable);
        }

        void ServerSingleton::networkUpdate(const Kiaro::Common::F32 &deltaTimeSeconds)
        {
            // Dispatch commit packets after we're done dispatching sim updates
            Kiaro::Game::Messages::SimCommit commitPacket;
            this->globalSend(&commitPacket, true);

            ENetEvent event;
            while (enet_host_service(mInternalHost, &event, 0) > 0)
            {
                switch(event.type)
                {
                    case ENET_EVENT_TYPE_CONNECT:
                    {
                        IncomingClient *client = new Kiaro::Network::IncomingClient(event.peer, this);
                        event.peer->data = client;

                        mConnectedClientSet.insert(mConnectedClientSet.end(), client);
                        onClientConnected(client);

                        break;
                    }

                    case ENET_EVENT_TYPE_DISCONNECT:
                    {
                        Kiaro::Network::IncomingClient *disconnected = (Kiaro::Network::IncomingClient*)event.peer->data;
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
                            throw std::runtime_error("ServerSingleton: Invalid ENet peer data on packet receive!");
                        }

                        Kiaro::Network::IncomingClient *sender = (Kiaro::Network::IncomingClient*)event.peer->data;
                        Kiaro::Support::BitStream incomingStream(event.packet->data, event.packet->dataLength);

                        onReceivePacket(incomingStream, sender);
                        enet_packet_destroy(event.packet);

                        break;
                    }

                    case ENET_EVENT_TYPE_NONE:
                        break;
                }
            }
        }

        void ServerSingleton::onClientConnected(Kiaro::Network::IncomingClient *client)
        {
            // Can we accept this guy?
            if (mConnectedClientSet.size() >= mMaximumClientCount)
            {
                client->disconnect("Server is full. ");
                return;
            }

            // Call the Lua callback
            lua_State *lua = Kiaro::Engine::CoreSingleton::getPointer()->getLuaState();
            lua_getglobal(lua, "GameServer");
            lua_getfield(lua, -1, "onClientConnected");

            lua_pushclient(lua, client);
            lua_call(lua, 1, 0);

            Kiaro::Support::EventManagerSingleton::get()->mOnClientConnectedEvent.invoke(client);
            std::cout << "ServerSingleton: Received remote connection from " << client->getStringIPAddress() << ":" << client->getPort() << std::endl;
        }

        void ServerSingleton::onClientDisconnected(Kiaro::Network::IncomingClient *client)
        {
            lua_State *lua = Kiaro::Engine::CoreSingleton::getPointer()->getLuaState();
            lua_getglobal(lua, "GameServer");
            lua_getfield(lua, -1, "onClientDisconnected");

            lua_pushclient(lua, client);
            lua_call(lua, 1, 0);

            Kiaro::Support::EventManagerSingleton::get()->mOnClientDisconnectedEvent.invoke(client);
            std::cout << "ServerSingleton: Received disconnection from " << client->getStringIPAddress() << ":" << client->getPort() << std::endl;
        }

        void ServerSingleton::onReceivePacket(Kiaro::Support::BitStream &incomingStream, Kiaro::Network::IncomingClient *sender)
        {
            mLastPacketSender = sender;

            Kiaro::Network::MessageBase basePacket;
            basePacket.unpackData(incomingStream);

            switch (basePacket.getType())
            {
                case Kiaro::Game::Messages::MESSAGE_HANDSHAKE:
                {
                    Kiaro::Game::Messages::HandShake receivedHandshake;
                    receivedHandshake.unpackData(incomingStream);

                    std::cout << "ServerSingleton: Client Version is " << (Kiaro::Common::U32)receivedHandshake.mVersionMajor << "."
                    << (Kiaro::Common::U32)receivedHandshake.mVersionMinor << "." << (Kiaro::Common::U32)receivedHandshake.mVersionRevision << "."
                    << (Kiaro::Common::U32)receivedHandshake.mVersionBuild << std::endl;

                    Kiaro::Game::Messages::HandShake handShake;
                    handShake.mVersionMajor = 1;
                    handShake.mVersionMinor = 2;
                    handShake.mVersionRevision = 3;
                    handShake.mVersionBuild = 4;

                    sender->send(&handShake, true);
                    break;
                }
            }

            mLastPacketSender = NULL;
        }

        Kiaro::Network::IncomingClient *ServerSingleton::getLastPacketSender(void)
        {
            Kiaro::Network::IncomingClient *result = mLastPacketSender;
            mLastPacketSender = NULL;

            return result;
        }

        void ServerSingleton::stop(void) { mIsRunning = false; }

        Kiaro::Common::U32 ServerSingleton::getClientCount(void)
        {
            return mConnectedClientSet.size();
        }

        void ServerSingleton::setGamemode(Kiaro::Game::GamemodeBase *game)
        {
            if (mCurrentGamemode)
                mCurrentGamemode->tearDown();

            delete mCurrentGamemode;
            mCurrentGamemode = game;

            mCurrentGamemode->setup();
        }

        void ServerSingleton::dispatch(void)
        {
            if (mIsRunning)
                enet_host_flush(mInternalHost);
        }
    } // End Namespace Game
} // End Namespace Kiaro
