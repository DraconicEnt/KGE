/**
 *  @file CClient.h
 *  @brief Include file declaring the CIncomingClient type.
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
        class IServer;

        namespace Messages
        {
            class IMessage;
        }

        //! The CIncomingClient class is a handle for a remote host that has connected to the game server.
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

                /**
                 *  @brief Sends a message to the given CIncomingClient.
                 *  @param message A pointer to the message to be sent.
                 *  @param reliable A boolean representing whether or not the
                 *  message should be sent reliably.
                 */
                void send(Messages::IMessage* message, const bool& reliable) NOTHROW;

                bool getIsOppositeEndian(void) NOTHROW;

                //! Disconnects this client from the server.
                void disconnect(const Support::String &reason) NOTHROW;

                /**
                 *  @brief Get the port number that this CIncomingClient is connecting on.
                 *  @return A Common::U16 representing the port number that this CIncomingClient is
                 *  connected on.
                 */
                const Common::U16 &getPort(void) NOTHROW;

                const Common::U32 &getBinaryIPAddress(void) NOTHROW;
                Support::String getStringIPAddress(void);

                void setStage(const Common::U32& in);
                const Common::U32& getStage(void);

            private:
                //! A boolean representing whether or not this connecting client has the opposite endianness.
                bool mIsOppositeEndian;
                ENetPeer *mInternalClient;

                //! What stage is this client currently operating in?
                Common::U32 mCurrentStage;
        };
    } // End Namespace Network
} // End Namespace Kiaro
#endif // _INCLUDE_NETWORK_CINCOMINGCLIENT_HPP_
