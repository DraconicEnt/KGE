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

#include <game/SGameWorld.hpp>

#include <game/SEventManager.hpp>

#include <game/messages/HandShake.hpp>
#include <game/messages/SimCommit.hpp>
#include <game/messages/Disconnect.hpp>
#include <net/SServer.hpp>

#include <net/CClient.hpp>

#include <game/entities/CSky.hpp>

#include <support/CMapDivision.hpp>

#include <game/IGameMode.hpp>

#include <lualibs/clientlib.h>

namespace Kiaro
{
    namespace Net
    {
        SServer* sInstance = NULL;

        SServer* SServer::getPointer(const Support::String& listenAddress, const Common::U16& listenPort, const Common::U32& maximumClientCount)
        {
            if (!sInstance)
                sInstance = new SServer(listenAddress, listenPort, maximumClientCount);

            return sInstance;
        }

        void SServer::destroy(void)
        {
            if (sInstance)
                delete sInstance;

            sInstance = NULL;
        }

        SServer::SServer(const Support::String& listenAddress, const Common::U16& listenPort, const Common::U32& maximumClientCount) :
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

            std::cout << "SERV: " << lua_gettop(luaState) << std::endl;

            // Create the map division
            Support::CMapDivision::Get(12);

            mEntityGroup = Game::SGameWorld::getPointer();

            // All entities register themselves with the Entity grouping singleton
            new Game::Entities::CSky();

            // Create an init a gamemode
        }

        SServer::~SServer(void)
        {
            // Call the server shutdown sequence in Lua
            lua_State* lua = Core::SEngineInstance::getPointer()->getLuaState();
            lua_getglobal(lua, "GameServer");
            lua_getfield(lua, -1, "onShutdown");
            lua_call(lua, 0, 0);

            // Disconnect everyone
            for (Net::SServer::clientIterator it = this->clientsBegin(); it != this->clientsEnd(); it++)
                (*it)->disconnect("Server Shutdown");

            Game::SGameWorld::destroy();

            if (mInternalHost)
            {
                enet_host_flush(mInternalHost); // Make sure we dispatch disconnects
                enet_host_destroy(mInternalHost);
                mInternalHost = NULL;
            }

            mIsRunning = false;

            Support::CMapDivision::Destroy();
        }

        void SServer::update(const Common::F32& deltaTimeSeconds)
        {
            Net::SServer::networkUpdate(deltaTimeSeconds);

            mEntityGroup->update(deltaTimeSeconds);
        }

        void SServer::globalSend(Net::IMessage* packet, const bool& reliable)
        {
            for (std::set<Net::CClient*>::iterator it = mConnectedClientSet.begin(); it != mConnectedClientSet.end(); it++)
                (*it)->send(packet, reliable);
        }

        void SServer::networkUpdate(const Common::F32& deltaTimeSeconds)
        {
            // Dispatch commit packets after we're done dispatching sim updates
            Game::Messages::SimCommit commitPacket;
            this->globalSend(&commitPacket, true);

            ENetEvent event;
            while (enet_host_service(mInternalHost, &event, 0) > 0)
            {
                switch(event.type)
                {
                    case ENET_EVENT_TYPE_CONNECT:
                    {
                        CClient* client = new Net::CClient(event.peer, this);
                        event.peer->data = client;

                        mConnectedClientSet.insert(mConnectedClientSet.end(), client);
                        onClientConnected(client);

                        break;
                    }

                    case ENET_EVENT_TYPE_DISCONNECT:
                    {
                        Net::CClient* disconnected = (Net::CClient*)event.peer->data;
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

                        Net::CClient* sender = (Net::CClient*)event.peer->data;
                        Support::CBitStream incomingStream(event.packet->data, event.packet->dataLength);

                        onReceivePacket(incomingStream, sender);
                        enet_packet_destroy(event.packet);

                        break;
                    }

                    case ENET_EVENT_TYPE_NONE:
                        break;
                }
            }
        }

        void SServer::onClientConnected(Net::CClient *client)
        {
            // Can we accept this guy?
            if (mConnectedClientSet.size() >= mMaximumClientCount)
            {
                client->disconnect("Server is full. ");
                return;
            }

            // Call the Lua callback
            lua_State *lua = Core::SEngineInstance::getPointer()->getLuaState();
            lua_getglobal(lua, "GameServer");
            lua_getfield(lua, -1, "onClientConnected");

            lua_pushclient(lua, client);
            lua_call(lua, 1, 0);

            Core::SEventManager::get()->mOnClientConnectedEvent.invoke(client);
            std::cout << "SServer: Received remote connection from " << client->getStringIPAddress() << ":" << client->getPort() << std::endl;
        }

        void SServer::onClientDisconnected(Net::CClient *client)
        {
            lua_State* lua = Core::SEngineInstance::getPointer()->getLuaState();
            lua_getglobal(lua, "GameServer");
            lua_getfield(lua, -1, "onClientDisconnected");

            lua_pushclient(lua, client);
            lua_call(lua, 1, 0);

            Core::SEventManager::get()->mOnClientDisconnectedEvent.invoke(client);
            std::cout << "SServer: Received disconnection from " << client->getStringIPAddress() << ":" << client->getPort() << std::endl;
        }

        void SServer::onReceivePacket(Support::CBitStream &incomingStream, Net::CClient *sender)
        {
            mLastPacketSender = sender;

            Net::IMessage basePacket;
            basePacket.extractFrom(incomingStream);

            switch (basePacket.getType())
            {
                case Game::Messages::MESSAGE_HANDSHAKE:
                {
                    Game::Messages::HandShake receivedHandshake;
                    receivedHandshake.extractFrom(incomingStream);

                    std::cout << "SServer: Client Version is " << (Common::U32)receivedHandshake.mVersionMajor << "."
                    << (Common::U32)receivedHandshake.mVersionMinor << "." << (Common::U32)receivedHandshake.mVersionRevision << "."
                    << (Common::U32)receivedHandshake.mVersionBuild << std::endl;

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

        Net::CClient* SServer::getLastPacketSender(void)
        {
            Net::CClient* result = mLastPacketSender;
            mLastPacketSender = NULL;

            return result;
        }

        void SServer::stop(void) { mIsRunning = false; }

        Common::U32 SServer::getClientCount(void)
        {
            return mConnectedClientSet.size();
        }

        void SServer::setGamemode(Game::IGameMode* game)
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
