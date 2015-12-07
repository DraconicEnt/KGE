/**
 *  @file CClient.h
 *  @brief Include file declaring the CIncomingClient type.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2015 Draconic Entity
 */

#ifndef _INCLUDE_NET_CINCOMINGCLIENT_HPP_
#define _INCLUDE_NET_CINCOMINGCLIENT_HPP_

#include <enet/enet.h>

#include <support/String.hpp>
#include <support/common.hpp>

#include <net/stages.hpp>

namespace Kiaro
{
    namespace Support
    {
        class CBitStream;
    } // End NameSpace Support

    namespace Net
    {
        class IServer;

        namespace Messages
        {
            class IMessage;
        }

        //! The CIncomingClient class is a handle for a remote host that has connected to the game server.
        class CIncomingClient
        {
            // Public Members
            public:
                bool mIsConnected;
                
            // Private Members
            private:
                //! A pointer to the internally used ENet peer.
                ENetPeer* mInternalClient;

                //! A boolean representing whether or not this CIncomingClient has the opposite endianness.
                bool mIsOppositeEndian;

                /**
                 *  @brief What stage is this client currently operating in?
                 *  @warning This value can very easily go out of sync between the client and server
                 *  because of packet loss and network latency. All netcode that causes stage transitions
                 *  will need to account for these situations accordingly. If this value were to go be
                 *  desynchronized, it is extremely likely that an out of stage error is going to be raised
                 *  on either end, causing a client disconnect.
                 */
                STAGE_NAME mCurrentConnectionStage;
            
            // Public Methods
            public:
                /**
                 *  @brief Constructor accepting an ENetPeer object pointer.
                 *  @param connecting A Peer object that is connecting.
                 */
                CIncomingClient(ENetPeer *connecting, Net::IServer *server);

                //! Standard destructor.
                ~CIncomingClient(void);

                /**
                 *  @brief Sends a message to the given CIncomingClient.
                 *  @param message A pointer to the message to be sent.
                 *  @param reliable A boolean representing whether or not the
                 *  message should be sent reliably.
                 */
                void send(Messages::IMessage* message, const bool& reliable) NOTHROW;

                bool getIsOppositeEndian(void) NOTHROW;

                //! Disconnects this client from the server.
                void disconnect(const Support::String& reason) NOTHROW;

                /**
                 *  @brief Get the port number that this CIncomingClient is connecting on.
                 *  @return A Common::U16 representing the port number that this CIncomingClient is
                 *  connected on.
                 */
                const Common::U16& getPort(void) NOTHROW;

                /**
                 *  @brief Get the IP address that this CIncomingClient is connecting from.
                 *  @return A Common::U32 representing the IP address that this CIncomingClient is
                 *  connected from.
                 */
                const Common::U32& getIPAddress(void) NOTHROW;

                /**
                 *  @brief Gets the IP address that this CIncomingClient is connecting on and
                 *  returns it as a string using dotted decimal notation.
                 *  @return A Common::String representing the dotted decimal notation of the
                 *  IP address that this CIncomingClient is connecting from.
                 */
                Support::String getIPAddressString(void);

                /**
                 *  @brief Sets the current connection stage of this CIncomingClient.
                 *  @param in The stage to set this CIncomingClient to.
                 *  @warning This has no affect on the client on the remote end; this is merely
                 *  a tracker variable on the server side. It is up to the netcode to keep these
                 *  two values in sync.
                 *  @see mCurrentConnectionStage
                 */
                void setConnectionStage(const STAGE_NAME& in);

                /**
                 *  @brief Gets the current connection stage of this CIncomingClient.
                 *  @return The current connection stage of this CIncomingClient.
                 *  @see mCurrentConnectionStage
                 */
                const STAGE_NAME& getConnectionStage(void);
        };
    } // End Namespace Network
} // End Namespace Kiaro
#endif // _INCLUDE_NET_CINCOMINGCLIENT_HPP_
