/**
 *  @file CClient.h
 *  @brief Main include file defining "global" scope functions and including sub files.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_NETWORK_CINCOMINGCLIENT_HPP_
#define _INCLUDE_NETWORK_CINCOMINGCLIENT_HPP_

#include <enet/enet.h>

#include <support/String.hpp>
#include <support/common.hpp>

namespace Kiaro
{
    namespace Support
    {
        class CBitStream;
    } // End NameSpace Support

    namespace Net
    {
        class IMessage;
        class IServer;

        //! The CClient class is a handle for a remote host that has connected to the game server.
        class CIncomingClient
        {
            public:
                /**
                 *  @brief Constructor accepting a Peer object.
                 *  @param connecting A Peer object that is connecting.
                 */
                CIncomingClient(ENetPeer *connecting, Net::IServer *server);

                //! Standard destructor.
                ~CIncomingClient(void);

                void send(Net::IMessage *packet, const bool &reliable) NOTHROW;

                bool getIsOppositeEndian(void) NOTHROW;

                //! Disconnects this client from the server.
                void disconnect(const Support::String &reason) NOTHROW;

                //! Get the port number that this client is connecting on.
                const Kiaro::Common::U16 &getPort(void) NOTHROW;

                const Kiaro::Common::U32 &getBinaryIPAddress(void) NOTHROW;
                Support::String getStringIPAddress(void);

            private:
                //! A boolean representing whether or not this connecting client has the opposite endianness.
                bool mIsOppositeEndian;
                ENetPeer *mInternalClient;
        };
    } // End Namespace Network
} // End Namespace Kiaro
#endif // _INCLUDE_NETWORK_CINCOMINGCLIENT_HPP_
