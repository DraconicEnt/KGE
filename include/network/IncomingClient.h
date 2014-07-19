/**
 *  @file IncomingClient.h
 *  @brief Main include file defining "global" scope functions and including sub files.
 *
 *  This software is licensed under the GNU Lesser General Public License version 3.
 *  Please refer to gpl.txt and lgpl.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @version 0.1.0
 *  @date 12/23/2014
 *  @copyright (c) 2013 Draconic Entertainment
 */

#ifndef _INCLUDE_NETWORK_INCOMINGCLIENT_H_
#define _INCLUDE_NETWORK_INCOMINGCLIENT_H_

#include "Packet.h"

namespace Kiaro
{
    namespace Network
    {
        class Server;

        //! The RemoteClient class is merely used to differentiate between a Client instance we created and a connected remote host in code.
        class IncomingClient
        {
            public:
            /**
             *  @brief Constructor accepting a Peer object.
             *  @param connecting A Peer object that is connecting.
             */
            IncomingClient(ENetPeer *connecting, Kiaro::Network::Server *server);

            ~IncomingClient(void);

            /**
             *  @brief Empty callback function for the the OnReceivePacket event.
             *  @param packet The received packet.
             */
            void OnReceivePacket(Kiaro::Network::Packet *packet);

            void Send(const Kiaro::Common::U32 size, const void *data, const bool reliable);

            private:

            Kiaro::Network::Server *Server;
            ENetPeer *Client;
        };
    } // End Namespace Network
} // End Namespace Kiaro
#endif // _INCLUDE_NETWORK_INCOMINGCLIENT_H_
