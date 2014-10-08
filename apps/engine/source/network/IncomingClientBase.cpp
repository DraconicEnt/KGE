/**
 *  @file IncomingClientBase.cpp
 *  @brief Source code file defining logic for the Kiaro::Network::OutgoingClient class.
 *
 *  This software is licensed under the GNU Lesser General Public License version 3.
 *  Please refer to gpl.txt and lgpl.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @version 0.1.0
 *  @date 3/5/2014
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>

#include <network/IncomingClientBase.hpp>

namespace Kiaro
{
    namespace Network
    {
        IncomingClientBase::IncomingClientBase(ENetPeer *connecting, Kiaro::Network::ServerBase *server) : mInternalClient(connecting)
        {

        }

        IncomingClientBase::~IncomingClientBase(void)
        {

        }

        void IncomingClientBase::onReceivePacket(Kiaro::Support::BitStream &incomingStream) { }

        void IncomingClientBase::send(Kiaro::Network::PacketBase *packet, const bool &reliable)
        {
            Kiaro::Common::U32 packet_flag = ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT;
            if (reliable)
                packet_flag = ENET_PACKET_FLAG_RELIABLE;

                // TODO: Packet Size Query
                Kiaro::Support::BitStream outStream(NULL, 0, 0);
                packet->packData(outStream);

                ENetPacket *enetPacket = enet_packet_create(outStream.raw(), outStream.length(), packet_flag);
                enet_peer_send(mInternalClient, 0, enetPacket);
        }

        bool IncomingClientBase::getIsOppositeEndian(void) { return mIsOppositeEndian; }

        void IncomingClientBase::disconnect(void)
        {
            enet_peer_disconnect_now(mInternalClient, 0);
        }

        Kiaro::Common::U16 IncomingClientBase::getPort(void)
        {
            return mInternalClient->address.port;
        }
    } // End Namespace Network
} // End Namespace Kiaro
