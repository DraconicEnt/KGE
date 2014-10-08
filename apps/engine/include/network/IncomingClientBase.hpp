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

#ifndef _INCLUDE_NETWORK_INCOMINGCLIENTBASE_HPP_
#define _INCLUDE_NETWORK_INCOMINGCLIENTBASE_HPP_

#include <enet/enet.h>

#include <network/ClientBase.hpp>
#include "PacketBase.hpp"

namespace Kiaro
{
    namespace Support
    {
        class BitStream;
    } // End NameSpace Support

    namespace Network
    {
        class ServerBase;
        class PacketBase;

        //! The RemoteClient class is merely used to differentiate between a Client instance we created and a connected remote host in code.
        class IncomingClientBase : public ClientBase
        {
            public:
                /**
                 *  @brief Constructor accepting a Peer object.
                 *  @param connecting A Peer object that is connecting.
                 */
                IncomingClientBase(ENetPeer *connecting, Kiaro::Network::ServerBase *server);

                ~IncomingClientBase(void);

                /**
                 *  @brief Empty callback function for the the OnReceivePacket event.
                 *  @param packet The received packet.
                 */
                void onReceivePacket(Kiaro::Support::BitStream &incomingStream);

                void send(Kiaro::Network::PacketBase *packet, const bool &reliable);

                bool getIsOppositeEndian(void);

                void disconnect(void);

                Kiaro::Common::U16 getPort(void);

            private:
                bool mIsOppositeEndian;
                ENetPeer *mInternalClient;
        };
    } // End Namespace Network
} // End Namespace Kiaro
#endif // _INCLUDE_NETWORK_INCOMINGCLIENTBASE_HPP_
