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

#include <support/Console.hpp>

//#include <core/Logging.hpp>
//#include <video/CBulletDebugDrawer.hpp>
//#include <game/SGameWorld.hpp>
//#include <core/SEngineInstance.hpp>

//#include <game/MoveManager.hpp>

#include <net/IOutgoingClient.hpp>

#include <support/CBitStream.hpp>

#include <net/messages/messages.hpp>

namespace Kiaro
{
    namespace Net
    {
        IOutgoingClient::IOutgoingClient() : mPort(0), mCurrentStage(0), mIsConnected(false), mInternalPeer(NULL), mInternalHost(NULL)
        {
        /*
            mEntityGroup = Game::SGameWorld::getPointer();

            mBroadphase = new btDbvtBroadphase();
            mCollisionConfiguration = new btDefaultCollisionConfiguration();
            mPhysicalDebugger = new Video::CBulletDebugDrawer(irrlicht);
            mCollisionDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
            mConstraintSolver = new btSequentialImpulseConstraintSolver();

            mPhysicalWorld = new btDiscreteDynamicsWorld(mCollisionDispatcher, mBroadphase, mConstraintSolver, mCollisionConfiguration);
            mPhysicalWorld->setDebugDrawer(mPhysicalDebugger);

            Core::Logging::write(Core::Logging::MESSAGE_INFO, "SClient: Initialized Bullet.");
            */
        }

        IOutgoingClient::~IOutgoingClient(void)
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

/*
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
            */
        }

        void IOutgoingClient::internalOnConnected(void)
        {
            Support::Console::write(Support::Console::MESSAGE_INFO, "IOutgoingClient: Established connection to remote host.");

            // Dispatch our own handshake in response
            Net::Messages::HandShake handShake;

            this->send(&handShake, true);

            //lua_State* lua = Core::SEngineInstance::getPointer()->getLuaState();

            //EasyLua::pushObject(lua, "GameClient", "onConnected");

            //if (lua_pcall(lua, 0, 0, 0))
             //   Core::Logging::write(Core::Logging::MESSAGE_ERROR, "SClient: %s", lua_tostring(lua, -1));
        }

/*
        void IOutgoingClient::onDisconnected(void)
        {
            Core::Logging::write(Core::Logging::MESSAGE_INFO, "SClient: Disconnected from remote host.");

            lua_State *lua = Core::SEngineInstance::getPointer()->getLuaState();

            EasyLua::pushObject(lua, "GameClient", "onDisconnected");

            EasyLua::call(lua, 1.02f, "Test", 5);
            //lua_call(lua, 0, 0);
        }
        */

/*
        void IOutgoingClient::onConnectFailed(void)
        {
            lua_State* lua = Core::SEngineInstance::getPointer()->getLuaState();

            EasyLua::pushObject(lua, "GameClient", "onConnectFailed");
            lua_call(lua, 0, 0);
        }
        */

        void IOutgoingClient::send(Messages::IMessage* packet, const bool& reliable)
        {
            Common::U32 packetFlag = ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT;
            if (reliable)
                packetFlag = ENET_PACKET_FLAG_RELIABLE;

            // TODO: Packet Size Query
            Support::CBitStream outStream(packet);

            ENetPacket* enetPacket = enet_packet_create(outStream.getBlock(), outStream.getPointer(), packetFlag);
            enet_peer_send(mInternalPeer, 0, enetPacket);
        }

        void IOutgoingClient::connect(const Support::String& hostName, const Common::U16& targetPort, const Common::U32& wait)
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

      //      Core::Logging::write(Core::Logging::MESSAGE_INFO, "SClient: Attempting connection to remote host %s:%u.", targetAddress.data(), targetPort);

            ENetAddress enetAddress;
            enet_address_set_host(&enetAddress, hostName.c_str());
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

                this->internalOnConnected();

                //this->address = enet_address.host;
                mPort = targetPort;
                return;
            }

            Support::Console::write(Support::Console::MESSAGE_ERROR, "SEngineInstance: Failed to connect to remote host.");

            this->onConnectFailed();
            enet_peer_reset(mInternalPeer);
        }

        void IOutgoingClient::disconnect(void)
        {
            if (!mIsConnected)
                return;

           // Game::MoveManager::reset();

            mIsConnected = false;
            enet_peer_disconnect_later(mInternalPeer, 0);
        }

        void IOutgoingClient::processPacket(Support::CBitStream& incomingStream)
        {
            while (!incomingStream.isEmpty())
            {
                Messages::IMessage basePacket;
                basePacket.unpack(incomingStream);

                switch (basePacket.getType())
                {
                    // Stageless messages
                    case Net::Messages::TYPE_DISCONNECT:
                    {
                        Net::Messages::Disconnect disconnect;
                        disconnect.unpack(incomingStream);

                        Support::Console::writef(Support::Console::MESSAGE_INFO, "IOutgoingClient: Received disconnect packet from remote host. Reason:\n%s", disconnect.mReason.data());
                        this->disconnect();

                        break;
                    }

                    // Either it's an unknown message or it's a staged message
                    default:
                    {
                        switch (mCurrentStage)
                        {
                            case 0:
                            {
                                this->processStageZero(basePacket, incomingStream);
                                break;
                            }
                        }
                    }
                }
            }
        }

        void IOutgoingClient::processStageZero(const Messages::IMessage& header, Support::CBitStream& incomingStream)
        {
            switch (header.getType())
            {
                case Messages::TYPE_HANDSHAKE:
                {
                    Messages::HandShake receivedHandshake;
                    receivedHandshake.unpack(incomingStream);

                    Support::Console::writef(Support::Console::MESSAGE_INFO, "IOutgoingClient: Server version is %u.%u.%u.%u.", receivedHandshake.mVersionMajor,
                    receivedHandshake.mVersionMinor, receivedHandshake.mVersionRevision, receivedHandshake.mVersionBuild);

                    Support::Console::write(Support::Console::MESSAGE_INFO, "IOutgoingClient: Passed initial authentication.");

                    mIsConnected = true;
                    this->onConnected();

                    mCurrentStage = 1;

                    break;
                }

                // Out of stage packet or unknown type
                default:
                {
                    Support::String exceptionText = "IOutgoingClient: Out of stage or unknown message type encountered at stage 0 processing: ";
                    exceptionText += header.getType();

                    throw std::out_of_range(exceptionText);
                    break;
                }
            }
        }

        void IOutgoingClient::processStageTwo(const Messages::IMessage& header, Support::CBitStream& incomingStream)
        {
            switch (header.getType())
            {
                case Net::Messages::TYPE_SCOPE:
                {
                    Net::Messages::Scope receivedScope;
                    receivedScope.unpack(incomingStream);

                    break;
                }

                case Net::Messages::TYPE_SIMCOMMIT:
                {
                    Net::Messages::SimCommit receivedCommit;
                    receivedCommit.unpack(incomingStream);

                    break;
                }

                // Out of stage packet or unknown type
                default:
                {
                    Support::String exceptionText = "IOutgoingClient: Out of stage or unknown message type encountered at stage 2 processing: ";
                    exceptionText += header.getType();

                    throw std::out_of_range(exceptionText);
                    break;
                }
            }
        }

        void IOutgoingClient::update(void)
        {
            if (!mIsConnected && !mInternalPeer)
                return;

            ENetEvent event;
            while (mInternalHost && enet_host_service(mInternalHost, &event, 0) > 0)
                switch(event.type)
                {
                    case ENET_EVENT_TYPE_DISCONNECT:
                    {
                        enet_host_destroy(mInternalHost);
                        mInternalHost = NULL;
                        enet_peer_reset(mInternalPeer);
                        mInternalPeer = NULL;

                        this->onDisconnected();
                        Support::Console::write(Support::Console::MESSAGE_INFO, "IOutgoingClient: Disconnected from remote host.");

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
                        this->processPacket(incomingStream);
                        enet_packet_destroy(event.packet);

                        break;
                    }

                    // Pipe down compiler warnings
                    case ENET_EVENT_TYPE_NONE:
                    case ENET_EVENT_TYPE_CONNECT:
                        break;
                }
        }

        const bool& IOutgoingClient::isConnected(void)
        {
            return mIsConnected;
        }

        void IOutgoingClient::dispatch(void)
        {
            if (mInternalHost)
                enet_host_flush(mInternalHost);
        }
    } // End Namespace Network
} // End Namespace Kiaro
