/**
 *  @file OutgoingClient.cpp
 *  @brief Source code file defining logic for the Kiaro::Network::OutgoingClient class.
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
#include <regex>

#include <enet/enet.h>

#include <support/BitStream.hpp>

#include <game/messages/messages.hpp>

#include <network/OutgoingClientBase.hpp>

namespace Kiaro
{
    namespace Network
    {
        OutgoingClientBase::OutgoingClientBase() : mIsConnected(false), mPort(0), mCurrentStage(0), mInternalPeer(NULL), mInternalHost(NULL)
        {

        }

        OutgoingClientBase::~OutgoingClientBase(void)
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
        }

        void OutgoingClientBase::send(Kiaro::Network::MessageBase *packet, const bool &reliable)
        {
            Kiaro::Common::U32 packetFlag = ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT;
            if (reliable)
                packetFlag = ENET_PACKET_FLAG_RELIABLE;

            // TODO: Packet Size Query
            Kiaro::Support::BitStream outStream(NULL, 0, 0);
            packet->packData(outStream);

            ENetPacket *enetPacket = enet_packet_create(outStream.raw(), outStream.getSize(), packetFlag);
            enet_peer_send(mInternalPeer, 0, enetPacket);
        }

        void OutgoingClientBase::connect(const Kiaro::Common::String &targetAddress, const Kiaro::Common::U16 &targetPort, const Kiaro::Common::U32 &wait)
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

        void OutgoingClientBase::disconnect(const Kiaro::Common::U32 wait)
        {
            mIsConnected = false;

            if (!mInternalHost || !mIsConnected)
                return;

            enet_peer_disconnect(mInternalPeer, 0);

            ENetEvent event;
			// TODO: Wait for disconnect event that doesn't come immediately?
            while (enet_host_service(mInternalHost, &event, wait) > 0)
                switch (event.type)
                {
                    case ENET_EVENT_TYPE_DISCONNECT:
                    {
                    	enet_host_destroy(mInternalHost);
						mInternalHost = NULL;

                        break;
                    }
                    case ENET_EVENT_TYPE_RECEIVE:
                        enet_packet_destroy(event.packet);

                        break;

                    // Shutup compiler warnings
                    case ENET_EVENT_TYPE_NONE:
                        break;
                    case ENET_EVENT_TYPE_CONNECT:
                        break;
                }

            onDisconnected();
        }

        void OutgoingClientBase::networkUpdate(void)
        {
            if (!mIsConnected)
                return;

            ENetEvent event;
            while (enet_host_service(mInternalHost, &event, 0) > 0)
            {
                switch(event.type)
                {
                    case ENET_EVENT_TYPE_DISCONNECT:
                    {
                        onDisconnected();

                        break;
                    }

                    case ENET_EVENT_TYPE_RECEIVE:
                    {
                        Kiaro::Support::BitStream incomingStream(event.packet->data, event.packet->dataLength, event.packet->dataLength);
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

        bool OutgoingClientBase::getIsConnected(void) { return mIsConnected; }

        void OutgoingClientBase::dispatch(void) { if (mInternalHost) enet_host_flush(mInternalHost); }
    } // End Namespace Network
} // End Namespace Kiaro
