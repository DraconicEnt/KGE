/**
 *  @file IncomingClient.cpp
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

#include <boost/regex.hpp>

#include <enet/enet.h>

#include <EngineLogging.h>
#include <network/IncomingClient.h>

#include <network/Server.h>

namespace Kiaro
{
    namespace Network
    {
        IncomingClient::IncomingClient(ENetPeer *connecting, Kiaro::Network::Server *server) : Client(connecting), Server(server)
        {

        }

        IncomingClient::~IncomingClient(void)
        {

        }

        void IncomingClient::OnReceivePacket(Kiaro::Network::Packet *packet)
        {

        }

        void IncomingClient::Send(const Kiaro::Common::U32 size, const void *data, const bool reliable)
        {
            Kiaro::Common::U32 packet_flag = ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT;
            if (reliable)
                packet_flag = ENET_PACKET_FLAG_RELIABLE;

            ENetPacket *packet = enet_packet_create(data, size, packet_flag);
            enet_peer_send(this->Client, 0, packet);
        }
    } // End Namespace Network
} // End Namespace Kiaro
