/**
 *  @file ClientBase.h
 *  @brief Main include file defining "global" scope functions and including sub files.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_NETWORK_CLIENTBASE_HPP_
#define _INCLUDE_NETWORK_CLIENTBASE_HPP_

#include <enet/enet.h>

#include "MessageBase.hpp"

namespace Kiaro
{
    namespace Network
    {
        class ServerBase;
        class PacketBase;

        //! The RemoteClient class is merely used to differentiate between a Client instance we created and a connected remote host in code.
        class ClientBase
        {
            public:
            /**
             *  @brief Constructor accepting a Peer object.
             *  @param connecting A Peer object that is connecting.
             */
            //ClientBase(ENetPeer *connecting, Kiaro::Network::ServerBase *server);

            //~ClientBase(void);

            /**
             *  @brief Empty callback function for the the OnReceivePacket event.
             *  @param packet The received packet.
             */
            virtual void onReceivePacket(Kiaro::Support::BitStream &incomingStream) = 0;

            virtual void send(Kiaro::Network::MessageBase *packet, const bool &reliable) = 0;

            //bool GetIsOppositeEndian(void) { return mIsOppositeEndian; }

            virtual void disconnect(void) = 0;

            virtual Kiaro::Common::U16 getPort(void) = 0;
        };
    } // End Namespace Network
} // End Namespace Kiaro
#endif // _INCLUDE_NETWORK_CLIENTBASE_HPP_
