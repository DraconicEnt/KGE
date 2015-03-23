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

#include <engine/SGameWorld.hpp>

#include <engine/SEventManager.hpp>

#include <game/messages/HandShake.hpp>
#include <game/messages/SimCommit.hpp>
#include <game/messages/Disconnect.hpp>
#include <network/SServer.hpp>

#include <network/CClient.hpp>

#include <game/entities/CSky.hpp>

#include <support/CMapDivision.hpp>

#include <game/IGameMode.hpp>

#include <lualibs/clientlib.hpp>

namespace Kiaro
{
    namespace Network
    {
        SServer *ServerSingleton_Instance = NULL;

        SServer *SServer::getPointer(const Kiaro::Common::String &listenAddress, const Kiaro::Common::U16 &listenPort, const Kiaro::Common::U32 &maximumClientCount)
        {
            if (!ServerSingleton_Instance)
                ServerSingleton_Instance = new SServer(listenAddress, listenPort, maximumClientCount);

            return ServerSingleton_Instance;
        }

        void SServer::destroy(void)
        {
            if (ServerSingleton_Instance)
                delete ServerSingleton_Instance;

            ServerSingleton_Instance = NULL;
        }

        SServer::SServer(const Kiaro::Common::String &listenAddress, const Kiaro::Common::U16 &listenPort, const Kiaro::Common::U32 &maximumClientCount) :
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

                throw std::runtime_error("SServer: Failed to create ENet host!");
            }

            // Setup some more of the Lua implements
            lua_State *luaState = Kiaro::Engine::SEngineInstance::getPointer()->getLuaState();

            lua_getglobal(luaState, "GameServer");
            lua_pushstring(luaState, "listenPort");
            lua_pushnumber(luaState, listenPort);
            lua_settable(luaState, -3);

            lua_pushstring(luaState, "listenAddress");
            lua_pushstring(luaState, listenAddress.c_str());
            lua_settable(luaState, -3);

            // The table "GameServer" is left so we chop it off
            lua_pop(luaState, -1);

            std::cout << "SERV: " << lua_gettop(luaState) << std::endl;

            // Create the map division
            Kiaro::Support::CMapDivision::Get(12);

            mEntityGroup = Kiaro::Engine::SGameWorld::getPointer();

            // All entities register themselves with the Entity grouping singleton
            new Kiaro::Game::Entities::CSky();

            // Create an init a gamemode
        }

        SServer::~SServer(void)
        {
            // Call the server shutdown sequence in Lua
            lua_State *lua = Kiaro::Engine::SEngineInstance::getPointer()->getLuaState();
            lua_getglobal(lua, "GameServer");
            lua_getfield(lua, -1, "onShutdown");
            lua_call(lua, 0, 0);

            // Disconnect everyone
            for (Kiaro::Network::SServer::clientIterator it = this->clientsBegin(); it != this->clientsEnd(); it++)
                (*it)->disconnect("Server Shutdown");

            Kiaro::Engine::SGameWorld::destroy();

            if (mInternalHost)
            {
                enet_host_flush(mInternalHost); // Make sure we dispatch disconnects
                enet_host_destroy(mInternalHost);
                mInternalHost = NULL;
            }

            mIsRunning = false;

            Kiaro::Support::CMapDivision::Destroy();
        }

        void SServer::update(const Kiaro::Common::F32 &deltaTimeSeconds)
        {
            Kiaro::Network::SServer::networkUpdate(deltaTimeSeconds);

            mEntityGroup->update(deltaTimeSeconds);
        }

        void SServer::globalSend(Kiaro::Network::IMessage *packet, const bool &reliable)
        {
            for (std::set<Kiaro::Network::CClient*>::iterator it = mConnectedClientSet.begin(); it != mConnectedClientSet.end(); it++)
                (*it)->send(packet, reliable);
        }

        void SServer::networkUpdate(const Kiaro::Common::F32 &deltaTimeSeconds)
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
                        CClient *client = new Kiaro::Network::CClient(event.peer, this);
                        event.peer->data = client;

                        mConnectedClientSet.insert(mConnectedClientSet.end(), client);
                        onClientConnected(client);

                        break;
                    }

                    case ENET_EVENT_TYPE_DISCONNECT:
                    {
                        Kiaro::Network::CClient *disconnected = (Kiaro::Network::CClient*)event.peer->data;
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
                            throw std::runtime_error("SServer: Invalid ENet peer data on packet receive!");
                        }

                        Kiaro::Network::CClient *sender = (Kiaro::Network::CClient*)event.peer->data;
                        Kiaro::Support::CBitStream incomingStream(event.packet->data, event.packet->dataLength);

                        onReceivePacket(incomingStream, sender);
                        enet_packet_destroy(event.packet);

                        break;
                    }

                    case ENET_EVENT_TYPE_NONE:
                        break;
                }
            }
        }

        void SServer::onClientConnected(Kiaro::Network::CClient *client)
        {
            // Can we accept this guy?
            if (mConnectedClientSet.size() >= mMaximumClientCount)
            {
                client->disconnect("Server is full. ");
                return;
            }

            // Call the Lua callback
            lua_State *lua = Kiaro::Engine::SEngineInstance::getPointer()->getLuaState();
            lua_getglobal(lua, "GameServer");
            lua_getfield(lua, -1, "onClientConnected");

            lua_pushclient(lua, client);
            lua_call(lua, 1, 0);

            Kiaro::Engine::SEventManager::get()->mOnClientConnectedEvent.invoke(client);
            std::cout << "SServer: Received remote connection from " << client->getStringIPAddress() << ":" << client->getPort() << std::endl;
        }

        void SServer::onClientDisconnected(Kiaro::Network::CClient *client)
        {
            lua_State *lua = Kiaro::Engine::SEngineInstance::getPointer()->getLuaState();
            lua_getglobal(lua, "GameServer");
            lua_getfield(lua, -1, "onClientDisconnected");

            lua_pushclient(lua, client);
            lua_call(lua, 1, 0);

            Kiaro::Engine::SEventManager::get()->mOnClientDisconnectedEvent.invoke(client);
            std::cout << "SServer: Received disconnection from " << client->getStringIPAddress() << ":" << client->getPort() << std::endl;
        }

        void SServer::onReceivePacket(Kiaro::Support::CBitStream &incomingStream, Kiaro::Network::CClient *sender)
        {
            mLastPacketSender = sender;

            Kiaro::Network::IMessage basePacket;
            basePacket.unpackData(incomingStream);

            switch (basePacket.getType())
            {
                case Kiaro::Game::Messages::MESSAGE_HANDSHAKE:
                {
                    Kiaro::Game::Messages::HandShake receivedHandshake;
                    receivedHandshake.unpackData(incomingStream);

                    std::cout << "SServer: Client Version is " << (Kiaro::Common::U32)receivedHandshake.mVersionMajor << "."
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

        Kiaro::Network::CClient *SServer::getLastPacketSender(void)
        {
            Kiaro::Network::CClient *result = mLastPacketSender;
            mLastPacketSender = NULL;

            return result;
        }

        void SServer::stop(void) { mIsRunning = false; }

        Kiaro::Common::U32 SServer::getClientCount(void)
        {
            return mConnectedClientSet.size();
        }

        void SServer::setGamemode(Kiaro::Game::IGameMode *game)
        {
            if (mCurrentGamemode)
                mCurrentGamemode->tearDown();

            delete mCurrentGamemode;
            mCurrentGamemode = game;

            mCurrentGamemode->setup();
        }

        void SServer::dispatch(void)
        {
            if (mIsRunning)
                enet_host_flush(mInternalHost);
        }
    } // End Namespace Game
} // End Namespace Kiaro
