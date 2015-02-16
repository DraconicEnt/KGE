/**
 *  @file IncomingClient.h
 *  @brief Main include file defining "global" scope functions and including sub files.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_NETWORK_INCOMINGCLIENT_HPP_
#define _INCLUDE_NETWORK_INCOMINGCLIENT_HPP_

#include <enet/enet.h>

#include <engine/Common.hpp>

namespace Kiaro
{
    namespace Support
    {
        class BitStream;
    } // End NameSpace Support

    namespace Network
    {
        class MessageBase;
        class ServerSingleton;

        //! The RemoteClient class is merely used to differentiate between a Client instance we created and a connected remote host in code.
        class IncomingClient
        {
            public:
                /**
                 *  @brief Constructor accepting a Peer object.
                 *  @param connecting A Peer object that is connecting.
                 */
                IncomingClient(ENetPeer *connecting, Kiaro::Network::ServerSingleton *server);

                //! Standard destructor.
                ~IncomingClient(void);

                void send(Kiaro::Network::MessageBase *packet, const bool &reliable) NOTHROW;

                bool getIsOppositeEndian(void) NOTHROW;

                //! Disconnects this client from the server.
                void disconnect(const Kiaro::Common::String &reason) NOTHROW;

                //! Get the port number that this client is connecting on.
                const Kiaro::Common::U16 &getPort(void) NOTHROW;

                const Kiaro::Common::U32 &getBinaryIPAddress(void) NOTHROW;
                std::string getStringIPAddress(void);

            private:
                //! A boolean representing whether or not this connecting client has the opposite endianness.
                bool mIsOppositeEndian;
                ENetPeer *mInternalClient;
        };
    } // End Namespace Network
} // End Namespace Kiaro
#endif // _INCLUDE_NETWORK_INCOMINGCLIENT_HPP_
