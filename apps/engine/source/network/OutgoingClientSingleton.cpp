/**
 *  @file OutgoingClientSingleton.cpp
 *  @brief Source code file defining logic for the Kiaro::Network::OutgoingClientSingleton class.
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

#include <engine/BulletDebugDrawer.hpp>
#include <engine/EntityGroupingSingleton.hpp>
#include <engine/CoreSingleton.hpp>

#include <game/messages/messages.hpp>
#include <game/messages/Disconnect.hpp>
#include <game/messages/HandShake.hpp>
#include <game/messages/SimCommit.hpp>

#include <network/OutgoingClientSingleton.hpp>

#include <support/BitStream.hpp>

namespace Kiaro
{
    namespace Network
    {
        static Kiaro::Network::OutgoingClientSingleton *OutgoingClientSingleton_Instance = NULL;

        OutgoingClientSingleton::OutgoingClientSingleton(irr::IrrlichtDevice *irrlicht, ENetPeer *incoming, Kiaro::Network::ServerBase *server) : mIsConnected(false), mPort(0), mCurrentStage(0), mInternalPeer(NULL), mInternalHost(NULL)
        {
            mEntityGroup = Kiaro::Engine::EntityGroupingSingleton::getPointer();

            mBroadphase = new btDbvtBroadphase();
            mCollisionConfiguration = new btDefaultCollisionConfiguration();
            mPhysicalDebugger = new Kiaro::Engine::BulletDebugDrawer(irrlicht);
            mCollisionDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
            mConstraintSolver = new btSequentialImpulseConstraintSolver();

            mPhysicalWorld = new btDiscreteDynamicsWorld(mCollisionDispatcher, mBroadphase, mConstraintSolver, mCollisionConfiguration);
            mPhysicalWorld->setDebugDrawer(mPhysicalDebugger);

            std::cout << "OutgoingClientSingleton: Initialized Bullet" << std::endl;
        }

        OutgoingClientSingleton::~OutgoingClientSingleton(void)
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

            std::cout << "OutgoingClientSingleton: Deinitialized Bullet" << std::endl;

            Kiaro::Engine::EntityGroupingSingleton::destroy();
        }

        void OutgoingClientSingleton::onReceivePacket(Kiaro::Support::BitStream &incomingStream)
        {
            // We need to know what type of packet it is first
            Kiaro::Network::MessageBase basePacket;
            basePacket.unpackData(incomingStream);

            switch (basePacket.getType())
            {
                case Kiaro::Game::Messages::MESSAGE_HANDSHAKE:
                {
                    Kiaro::Game::Messages::HandShake receivedHandshake;
                    receivedHandshake.unpackData(incomingStream);

                    // NOTE: Would rather printf here but then the stdout override doesn't work
                    std::cout << "OutgoingClient: Server Version is " << (Kiaro::Common::U32)receivedHandshake.mVersionMajor << "."
                    << (Kiaro::Common::U32)receivedHandshake.mVersionMinor << "." << (Kiaro::Common::U32)receivedHandshake.mVersionRevision << "."
                    << (Kiaro::Common::U32)receivedHandshake.mVersionBuild << std::endl;

                    mCurrentStage = 1;

                    break;
                }

                case Kiaro::Game::Messages::MESSAGE_SIMCOMMIT:
                {
                    Kiaro::Game::Messages::SimCommit receivedCommit;
                    receivedCommit.unpackData(incomingStream);
                    break;
                }

                case Kiaro::Game::Messages::MESSAGE_DISCONNECT:
                {
                    Kiaro::Game::Messages::Disconnect disconnect;
                    disconnect.unpackData(incomingStream);

                    // TODO (Robert MacGregor#9): Call an onDisconnect method in Lua
                    std::cout << "OutgoingClientSingleton: Received disconnect packet from remote host. Reason: \"" << disconnect.mReason << "\"" << std::endl;
                    this->disconnect();

                    // TODO (Robert MacGregor#9): Safe to do from within the class like this?
                    Kiaro::Network::OutgoingClientSingleton::destroy();
                    break;
                }

                default:
                {
                    // FIXME (Robert MacGregor#9): Fails to print out the type identifier?
                    std::string errorString = "OutgoingClientSingleton: Received unknown packet. Type Identifier: ";
                    errorString += basePacket.getType();
                    throw std::logic_error(errorString);

                    break;
                }
            }
        }

        void OutgoingClientSingleton::onConnected(void)
        {
            std::cout << "OutgoingClient: Established connection to remote host" << std::endl;

            // Dispatch our own handshake in response
            Kiaro::Game::Messages::HandShake handShake;
            handShake.mVersionMajor = 1;
            handShake.mVersionMinor = 2;
            handShake.mVersionRevision = 3;
            handShake.mVersionBuild = 4;

            this->send(&handShake, true);
        }

        void OutgoingClientSingleton::onDisconnected(void)
        {
            std::cout << "OutgoingClient: Disconnected from the remote host" << std::endl;
        }

        void OutgoingClientSingleton::onConnectFailed(void)
        {

        }

        OutgoingClientSingleton *OutgoingClientSingleton::getPointer(irr::IrrlichtDevice *irrlicht)
        {
            if (!OutgoingClientSingleton_Instance)
                OutgoingClientSingleton_Instance = new OutgoingClientSingleton(irrlicht, NULL, NULL);

            return OutgoingClientSingleton_Instance;
        }

        void OutgoingClientSingleton::destroy(void)
        {
            if (OutgoingClientSingleton_Instance)
            {
                delete OutgoingClientSingleton_Instance;
                OutgoingClientSingleton_Instance = NULL;
            }
        }

        void OutgoingClientSingleton::send(Kiaro::Network::MessageBase *packet, const bool &reliable)
        {
            Kiaro::Common::U32 packetFlag = ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT;
            if (reliable)
                packetFlag = ENET_PACKET_FLAG_RELIABLE;

            // TODO: Packet Size Query
            Kiaro::Support::BitStream outStream;
            packet->packData(outStream);

            ENetPacket *enetPacket = enet_packet_create(outStream.getBasePointer(), outStream.getSize(), packetFlag);
            enet_peer_send(mInternalPeer, 0, enetPacket);
        }

        void OutgoingClientSingleton::connect(const Kiaro::Common::String &targetAddress, const Kiaro::Common::U16 &targetPort, const Kiaro::Common::U32 &wait)
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

            std::cout << "OutgoingClientBase: Attempting connection to remote host " << targetAddress << ":" << targetPort << std::endl;

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

            std::cerr << "OutgoingClientBase: Failed to connect to remote host" << std::endl;

            onConnectFailed();
            enet_peer_reset(mInternalPeer);
        }

        void OutgoingClientSingleton::disconnect(const Kiaro::Common::U32 &waitTimeMS)
        {
            if (!mIsConnected)
                return;

            mIsConnected = false;
            enet_peer_disconnect(mInternalPeer, 0);

            ENetEvent event;

			// TODO: Wait for disconnect event that doesn't come immediately?
            while (mInternalHost && enet_host_service(mInternalHost, &event, waitTimeMS) > 0)
                switch (event.type)
                {
                    case ENET_EVENT_TYPE_DISCONNECT:
                    {
                    	enet_host_destroy(mInternalHost);
						mInternalHost = NULL;

                        break;
                    }
                    case ENET_EVENT_TYPE_RECEIVE:
                    {
                        enet_packet_destroy(event.packet);

                        break;
                    }

                    // Shutup compiler warnings
                    case ENET_EVENT_TYPE_NONE:
                    case ENET_EVENT_TYPE_CONNECT:
                        break;
                }

            onDisconnected();
        }

        void OutgoingClientSingleton::networkUpdate(void)
        {
            if (!mIsConnected)
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

                        onDisconnected();

                        break;
                    }

                    case ENET_EVENT_TYPE_RECEIVE:
                    {
                        Kiaro::Support::BitStream incomingStream(event.packet->data, event.packet->dataLength);
                        onReceivePacket(incomingStream);

                        break;
                    }

                    // Pipe down compiler warnings
                    case ENET_EVENT_TYPE_NONE:
                    case ENET_EVENT_TYPE_CONNECT:
                        break;
                }
            }
        }

        const bool &OutgoingClientSingleton::getIsConnected(void)
        {
            return mIsConnected;
        }

        void OutgoingClientSingleton::dispatch(void)
        {
            if (mInternalHost)
                enet_host_flush(mInternalHost);
        }
    } // End Namespace Network
} // End Namespace Kiaro
