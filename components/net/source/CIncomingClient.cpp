/**
 *  @file CClient.cpp
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

#include <net/CIncomingClient.hpp>

#include <support/CBitStream.hpp>

#include <net/IMessage.hpp>

//#include <net/messages/Disconnect.hpp>

namespace Kiaro
{
    namespace Net
    {
        CIncomingClient::CIncomingClient(ENetPeer* connecting, IServer* server) : mInternalClient(connecting)
        {

        }

        CIncomingClient::~CIncomingClient(void)
        {
            this->disconnect("Destroyed net handle.");
        }

        void CIncomingClient::send(IMessage* packet, const bool& reliable)
        {
            Common::U32 packetFlag = ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT;

            if (reliable)
                packetFlag = ENET_PACKET_FLAG_RELIABLE;

            // TODO: Packet Size Query
            Support::CBitStream outStream(packet);

            ENetPacket* enetPacket = enet_packet_create(outStream.getBlock(), outStream.getWrittenLength(), packetFlag);
            enet_peer_send(mInternalClient, 0, enetPacket);
        }

        bool CIncomingClient::getIsOppositeEndian(void)
        {
            return mIsOppositeEndian;
        }

        void CIncomingClient::disconnect(const Support::String& reason)
        {
         //   Net::Messages::Disconnect disconnect;
         //   disconnect.mReason = reason;

          //  this->send(&disconnect, true);
          //  enet_peer_disconnect_later(mInternalClient, 0);
        }

        const Common::U16& CIncomingClient::getPort(void)
        {
            return mInternalClient->address.port;
        }

        const Common::U32& CIncomingClient::getBinaryIPAddress(void)
        {
            return mInternalClient->address.host;
        }

        Support::String CIncomingClient::getStringIPAddress(void)
        {
            Common::C8 temporaryBuffer[32];

            enet_address_get_host_ip(&mInternalClient->address, temporaryBuffer, 32);
            return temporaryBuffer;
        }
    } // End Namespace Network
} // End Namespace Kiaro
