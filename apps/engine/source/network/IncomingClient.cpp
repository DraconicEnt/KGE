/**
 *  @file IncomingClient.cpp
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

#include <network/IncomingClient.hpp>

#include <support/BitStream.hpp>

#include <network/MessageBase.hpp>

#include <game/messages/Disconnect.hpp>

namespace Kiaro
{
    namespace Network
    {
        IncomingClient::IncomingClient(ENetPeer *connecting, Kiaro::Network::ServerSingleton *server) : mInternalClient(connecting)
        {

        }

        IncomingClient::~IncomingClient(void)
        {

        }

        void IncomingClient::send(Kiaro::Network::MessageBase *packet, const bool &reliable)
        {
            Kiaro::Common::U32 packetFlag = ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT;

            if (reliable)
                packetFlag = ENET_PACKET_FLAG_RELIABLE;

            // TODO: Packet Size Query
            Kiaro::Support::BitStream outStream(NULL, 0, 0);
            packet->packData(outStream);

            ENetPacket *enetPacket = enet_packet_create(outStream.raw(), outStream.getSize(), packetFlag);
            enet_peer_send(mInternalClient, 0, enetPacket);
        }

        bool IncomingClient::getIsOppositeEndian(void)
        {
            return mIsOppositeEndian;
        }

        void IncomingClient::disconnect(const Kiaro::Common::String &reason)
        {
            Kiaro::Game::Messages::Disconnect disconnect;
            disconnect.mReason = reason;

            this->send(&disconnect, true);
            enet_peer_disconnect_later(mInternalClient, 0);
        }

        const Kiaro::Common::U16 &IncomingClient::getPort(void)
        {
            return mInternalClient->address.port;
        }

        const Kiaro::Common::U32 &IncomingClient::getBinaryIPAddress(void)
        {
            return mInternalClient->address.host;
        }

        std::string IncomingClient::getStringIPAddress(void)
        {
            Kiaro::Common::C8 temporaryBuffer[32];

            enet_address_get_host_ip(&mInternalClient->address, temporaryBuffer, 32);
            return temporaryBuffer;
        }
    } // End Namespace Network
} // End Namespace Kiaro
