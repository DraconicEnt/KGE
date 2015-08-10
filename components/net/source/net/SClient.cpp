/**
 *  @file SClient.cpp
 *  @brief Implementation file for the Network::SClient class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>

#include <enet/enet.h>

//#include <core/Logging.hpp>
//#include <video/CBulletDebugDrawer.hpp>
//#include <game/SGameWorld.hpp>
//#include <core/SEngineInstance.hpp>

#include <net/messages/messages.hpp>
#include <net/messages/Disconnect.hpp>
#include <net/messages/HandShake.hpp>
#include <net/messages/SimCommit.hpp>
#include <net/messages/Scope.hpp>
//#include <game/MoveManager.hpp>

#include <net/SClient.hpp>

#include <support/CBitStream.hpp>

#include <easylua.hpp>

namespace Kiaro
{
    namespace Net
    {
        static Net::SClient* sInstance = NULL;

        SClient::SClient(irr::IrrlichtDevice* irrlicht, ENetPeer* incoming, Net::ServerBase* server) : mIsConnected(false), mPort(0), mCurrentStage(0), mInternalPeer(NULL), mInternalHost(NULL)
        {
            mEntityGroup = Game::SGameWorld::getPointer();

            mBroadphase = new btDbvtBroadphase();
            mCollisionConfiguration = new btDefaultCollisionConfiguration();
            mPhysicalDebugger = new Video::CBulletDebugDrawer(irrlicht);
            mCollisionDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
            mConstraintSolver = new btSequentialImpulseConstraintSolver();

            mPhysicalWorld = new btDiscreteDynamicsWorld(mCollisionDispatcher, mBroadphase, mConstraintSolver, mCollisionConfiguration);
            mPhysicalWorld->setDebugDrawer(mPhysicalDebugger);

            Core::Logging::write(Core::Logging::MESSAGE_INFO, "SClient: Initialized Bullet.");
        }

        SClient::~SClient(void)
        {
            if (mInternalPeer)
            {
                enet_peer_reset(mInternalPeer);
                mInternalPeer = NULL;
            }

            if (mInternalHost)
            {
                enet_host_destroy(mInternalHost);
                mInternalHost = NULL;
            }

            delete mPhysicalWorld;
            mPhysicalWorld = NULL;

            delete mConstraintSolver;
            mConstraintSolver = NULL;

            delete mBroadphase;
            mBroadphase = NULL;

            delete mCollisionDispatcher;
            mCollisionDispatcher = NULL;

            delete mCollisionConfiguration;
            mCollisionConfiguration = NULL;

            delete mPhysicalDebugger;
            mPhysicalDebugger = NULL;

            Core::Logging::write(Core::Logging::MESSAGE_INFO, "SClient: Deinitialized Bullet.");

            Game::SGameWorld::destroy();
        }

        void SClient::onReceivePacket(Support::CBitStream& incomingStream)
        {
            // We need to know what type of packet it is first
            Net::IMessage basePacket;
            basePacket.extractFrom(incomingStream);

            switch (basePacket.getType())
            {
                case Game::Messages::MESSAGE_SCOPE:
                {
                    Game::Messages::Scope receivedScope;
                    receivedScope.extractFrom(incomingStream);

                    break;
                }

                case Game::Messages::MESSAGE_HANDSHAKE:
                {
                    Game::Messages::HandShake receivedHandshake;
                    receivedHandshake.extractFrom(incomingStream);

                    // NOTE: Would rather printf here but then the stdout override doesn't work
                    Core::Logging::write(Core::Logging::MESSAGE_INFO, "SClient: Server version is %u.%u.%u.%u.", receivedHandshake.mVersionMajor,
                    receivedHandshake.mVersionMinor, receivedHandshake.mVersionRevision, receivedHandshake.mVersionBuild);

                    mCurrentStage = 1;

                    break;
                }

                case Game::Messages::MESSAGE_SIMCOMMIT:
                {
                    Game::Messages::SimCommit receivedCommit;
                    receivedCommit.extractFrom(incomingStream);
                    break;
                }

                case Game::Messages::MESSAGE_DISCONNECT:
                {
                    Game::Messages::Disconnect disconnect;
                    disconnect.extractFrom(incomingStream);

                    // TODO (Robert MacGregor#9): Call an onDisconnect method in Lua
                    Core::Logging::write(Core::Logging::MESSAGE_INFO, "SEngineInstance: Received disconnect packet from remote host. Reason:\n%s", disconnect.mReason.data());
                    this->disconnect();

                    // TODO (Robert MacGregor#9): Safe to do from within the class like this?
                    Net::SClient::destroy();
                    break;
                }

                default:
                {
                    // FIXME (Robert MacGregor#9): Fails to print out the type identifier?
                    std::string errorString = "SClient: Received unknown packet. Type Identifier: ";
                    errorString += basePacket.getType();
                    throw std::logic_error(errorString);

                    break;
                }
            }
        }

        void SClient::onConnected(void)
        {
            Core::Logging::write(Core::Logging::MESSAGE_INFO, "SClient: Established connection to remote host.");

            // Dispatch our own handshake in response
            Game::Messages::HandShake handShake;

            this->send(&handShake, true);

            lua_State* lua = Core::SEngineInstance::getPointer()->getLuaState();

            EasyLua::pushObject(lua, "GameClient", "onConnected");

            if (lua_pcall(lua, 0, 0, 0))
                Core::Logging::write(Core::Logging::MESSAGE_ERROR, "SClient: %s", lua_tostring(lua, -1));
        }

        void SClient::onDisconnected(void)
        {
            Core::Logging::write(Core::Logging::MESSAGE_INFO, "SClient: Disconnected from remote host.");

            lua_State *lua = Core::SEngineInstance::getPointer()->getLuaState();

            EasyLua::pushObject(lua, "GameClient", "onDisconnected");

            EasyLua::call(lua, 1.02f, "Test", 5);
            //lua_call(lua, 0, 0);
        }

        void SClient::onConnectFailed(void)
        {
            lua_State* lua = Core::SEngineInstance::getPointer()->getLuaState();

            EasyLua::pushObject(lua, "GameClient", "onConnectFailed");
            lua_call(lua, 0, 0);
        }

        void SClient::initialize(irr::IrrlichtDevice* irrlicht)
        {
            if (!sInstance)
                sInstance = new SClient(irrlicht, NULL, NULL);
        }

        SClient* SClient::getPointer(void)
        {
            return sInstance;
        }

        void SClient::destroy(void)
        {
            if (sInstance)
            {
                sInstance->disconnect();

                delete sInstance;
                sInstance = NULL;
            }
        }

        void SClient::send(Net::IMessage* packet, const bool& reliable)
        {
            Common::U32 packetFlag = ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT;
            if (reliable)
                packetFlag = ENET_PACKET_FLAG_RELIABLE;

            // TODO: Packet Size Query
            Support::CBitStream outStream(packet);

            ENetPacket* enetPacket = enet_packet_create(outStream.getBlock(), outStream.getWrittenLength(), packetFlag);
            enet_peer_send(mInternalPeer, 0, enetPacket);
        }

        void SClient::connect(const Support::String& targetAddress, const Common::U16& targetPort, const Common::U32& wait)
        {
			// TODO: Report Error
			if (mInternalPeer || mInternalHost)
				return;

            // FIXME (Robert MacGregor#9): IP Octets >= 256
            // FIXME (Robert MacGregor#9): DNS Names (host.host.net)
            // TODO (Robert MacGregor#9): Update for std::regex
            //static const std::regex ipExpression("([0-9]+.[0-9]+.[0-9]+.[0-9])|([[:alpha:]]+.([[:alpha:]]+.)*[[:alpha:]]+)");
            //if (!std::regex_match(targetAddress, ipExpression))
            //{
            //    std::cerr << "OutgoingClientBase: Invalid remote host '" << targetAddress << "'!" << std::endl;
            //    return;
           // }

            Core::Logging::write(Core::Logging::MESSAGE_INFO, "SClient: Attempting connection to remote host %s:%u.", targetAddress.data(), targetPort);

            ENetAddress enetAddress;
            enet_address_set_host(&enetAddress, targetAddress.c_str());
            enetAddress.port = targetPort;

            mInternalHost = enet_host_create(NULL /* create a client host */,
                            1 /* only allow 1 outgoing connection */,
                            2 /* allow up 2 channels to be used, 0 and 1 */,
                            57600 / 8 /* 56K modem with 56 Kbps downstream bandwidth */,
                            14400 / 8 /* 56K modem with 14 Kbps upstream bandwidth */);

            mInternalPeer = enet_host_connect(mInternalHost, &enetAddress, 2, 0);

            ENetEvent event;
            if (enet_host_service(mInternalHost, &event, wait) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
            {
                mCurrentStage = 0;

                onConnected();
                mIsConnected = true;

                //this->address = enet_address.host;
                mPort = targetPort;
                return;
            }

            Core::Logging::write(Core::Logging::MESSAGE_ERROR, "SEngineInstance: Failed to connect to remote host.");

            onConnectFailed();
            enet_peer_reset(mInternalPeer);
        }

        void SClient::disconnect(void)
        {
            if (!mIsConnected)
                return;

            Game::MoveManager::reset();

            mIsConnected = false;
            enet_peer_disconnect_later(mInternalPeer, 0);
        }

        void SClient::networkUpdate(void)
        {
            if (!mIsConnected && !mInternalPeer)
                return;

            ENetEvent event;
            while (mInternalHost && enet_host_service(mInternalHost, &event, 0) > 0)
            {
                switch(event.type)
                {
                    case ENET_EVENT_TYPE_DISCONNECT:
                    {
                        enet_host_destroy(mInternalHost);
                        mInternalHost = NULL;

                        this->onDisconnected();

                        break;
                    }

                    case ENET_EVENT_TYPE_RECEIVE:
                    {
                        // We're disconnecting, so just destroy anything we receive in the meantime.
                        if (!mIsConnected)
                        {
                            enet_packet_destroy(event.packet);
                            break;
                        }

                        Support::CBitStream incomingStream(event.packet->dataLength, event.packet->data);
                        this->onReceivePacket(incomingStream);

                        break;
                    }

                    // Pipe down compiler warnings
                    case ENET_EVENT_TYPE_NONE:
                    case ENET_EVENT_TYPE_CONNECT:
                        break;
                }
            }
        }

        const bool& SClient::getIsConnected(void)
        {
            return mIsConnected;
        }

        void SClient::dispatch(void)
        {
            if (mInternalHost)
                enet_host_flush(mInternalHost);
        }
    } // End Namespace Network
} // End Namespace Kiaro
