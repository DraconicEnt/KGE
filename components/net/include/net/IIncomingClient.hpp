/**
 *  @file IIncomingClient.hpp
 *  @brief Include file declaring the CIncomingClient type.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_NET_IINCOMINGCLIENT_HPP_
#define _INCLUDE_NET_IINCOMINGCLIENT_HPP_

#include <enet/enet.h>

#include <support/String.hpp>
#include <support/common.hpp>
#include <support/CBitStream.hpp>

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
        class IMessage;

        //! The IIncomingClient class is a handle for a remote host that has connected to the game server.
        class IIncomingClient
        {
                // Public Members
            public:
                //! Are we currently connected somewhere?
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

                Support::CBitStream mOutputBitStream;

                // Public Methods
            public:
                /**
                 *  @brief Constructor accepting an ENetPeer object pointer.
                 *  @param connecting A Peer object that is connecting.
                 */
                IIncomingClient(ENetPeer* connecting, Net::IServer* server);

                //! Standard destructor.
                ~IIncomingClient(void);

                /**
                 *  @brief Sends a message to the given IIncomingClient.
                 *  @param message A pointer to the message to be sent.
                 *  @param reliable A boolean representing whether or not the
                 *  message should be sent reliably.
                 */
                void send(IMessage* message, const bool reliable) NOTHROW;

                bool getIsOppositeEndian(void) const NOTHROW;

                //! Disconnects this client from the server.
                virtual void disconnect(const Support::String& reason);

                /**
                 *  @brief Get the port number that this CIncomingClient is connecting on.
                 *  @return A Common::U16 representing the port number that this CIncomingClient is
                 *  connected on.
                 */
                Common::U16 getPort(void) const NOTHROW;

                /**
                 *  @brief Get the IP address that this CIncomingClient is connecting from.
                 *  @return A Common::U32 representing the IP address that this CIncomingClient is
                 *  connected from.
                 */
                Common::U32 getIPAddress(void) const NOTHROW;

                /**
                 *  @brief Gets the IP address that this CIncomingClient is connecting on and
                 *  returns it as a string using dotted decimal notation.
                 *  @return A Common::String representing the dotted decimal notation of the
                 *  IP address that this CIncomingClient is connecting from.
                 */
                Support::String getIPAddressString(void) const;

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
                const STAGE_NAME& getConnectionStage(void) const;
        };
    } // End Namespace Network
} // End Namespace Kiaro
#endif // _INCLUDE_NET_CINCOMINGCLIENT_HPP_
