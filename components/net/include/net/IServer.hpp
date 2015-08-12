/**
 *  @file ServerSingleton.hpp
 *  @brief Include file defining the Server and related functions.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_GAME_SERVERSINGLETON_HPP_
#define _INCLUDE_KIARO_GAME_SERVERSINGLETON_HPP_

#include <enet/enet.h>

#include <support/Set.hpp>
#include <support/String.hpp>
#include <support/CBitStream.hpp>

#include "support/common.hpp"

namespace Kiaro
{
    namespace Net
    {
        class IMessage;
        class CIncomingClient;

        //! Server class that remote hosts connect to.
        class IServer
        {
            // Public Typedefs
            public:
                typedef Support::Set<CIncomingClient*>::iterator clientIterator;
                typedef Support::Set<CIncomingClient*>::const_iterator clientConstIterator;

            // Public Methods
            public:
                /**
                 *  @brief Signals the server to stop running.
                 */
                void stop(void);

                void globalSend(Net::IMessage* packet, const bool &reliable);

                /**
                 *  @brief Returns the current running status of the server.
                 *  @return A boolean representing whether or not the server is running.
                 */
                bool isRunning(void);

                //! Performs an update time pulse on the server.
                void update(const Common::F32& deltaTimeSeconds);

                //! Causes the server to handle all queued network events immediately.
                void dispatch(void);

                void networkUpdate(const Common::F32& deltaTimeSeconds);

                /**
                 *  @brief Callback function that is called upon the server's underlaying
                 *  network subsystem accepting a remote host connection.
                 *  @param client A pointer to a Kiaro::Network::IncomingClientBase representing
                 *  the incoming connection.
                 */
                void onClientConnected(Net::CIncomingClient* client);

                /**
                 *  @brief Callback function that is called upon the server's underlaying
                 *  network subsystem disconnecting a remote host connection.
                 *  @param client A pointer to a Kiaro::Network::IncomingClientBase representing
                 *  the disconnected client.
                 */
                void onClientDisconnected(Net::CIncomingClient* client);

                /**
                 *  @brief Callback function that is called upon the server's underlaying
                 *  network subsystem receiving a packet.
                 *  @param incomingStream A reference to the Kiaro::Support::BitStream that can be
                 *  used to unpack the packet payload.
                 *  @param sender A pointer to a Kiaro::Network::IncomingClientBase representing
                 *  the sender of the packet.
                 */
                void onReceivePacket(Support::CBitStream& incomingStream, Net::CIncomingClient* sender);

                Net::CIncomingClient* getLastPacketSender(void);

                Common::U32 getClientCount(void);

               // Kiaro::Network::IncomingClientBase *GetLastPacketSender(void);
                clientIterator clientsBegin(void) { return mConnectedClientSet.begin(); }

                clientConstIterator clientsEnd(void) { return mConnectedClientSet.end(); }

            // Protected Methods
            protected:
                /**
                 *  @brief Constructor that initializes a server instance with the given information.
                 *  @param listen_address An array of c8 representing the IP address to listen on.
                 *  @param listen_port A Kiaro::Common::U16 representing the port number to listen on.
                 *  @param clients A Kiaro::Common::U32 representing the maximum number of clients to accept.
                 */
                IServer(const Support::String& listenAddress, const Common::U16& listenPort, const Common::U32& maximumClientCount);

                /**
                 *  @brief Standard virtual destructor.
                 *  @note The only reason this destructor is virtual is to prevent compiler warnings when deleting instances that
                 *  are children of this Server class.
                 */
                ~IServer(void);

            // Private Members
            private:
                Net::CIncomingClient* mLastPacketSender;

                bool mIsRunning;

                ENetHost* mInternalHost;

                //! The Port number that we're listening on.
                const Common::U16 mListenPort;
                //! The Address that we're listening on.
                const Support::String mListenAddress;

                Common::U32 mMaximumClientCount;

                Support::Set<CIncomingClient*> mConnectedClientSet;
        };
    } // End Namespace Network
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_GAME_SERVERSINGLETON_HPP_
