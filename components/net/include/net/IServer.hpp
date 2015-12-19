/**
 *  @file ServerSingleton.hpp
 *  @brief Include file defining the Server and related functions.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2015 Draconic Entity
 */

#ifndef _INCLUDE_KIARO_GAME_SERVERSINGLETON_HPP_
#define _INCLUDE_KIARO_GAME_SERVERSINGLETON_HPP_

#include <enet/enet.h>

#include <support/UnorderedSet.hpp>
#include <support/String.hpp>
#include <support/CBitStream.hpp>

#include "support/common.hpp"

namespace Kiaro
{
    namespace Net
    {
        class IMessage;
        class IIncomingClient;

        typedef ENetPeer* RemoteHostContext;

        //! Server class that remote hosts connect to.
        class IServer
        {
            // Public Typedefs
            public:
                typedef Support::UnorderedSet<IIncomingClient*>::iterator clientIterator;
                typedef Support::UnorderedSet<IIncomingClient*>::const_iterator clientConstIterator;

            // Public Methods
            public:
                /**
                 *  @brief Signals the server to stop running.
                 */
                void stop(void);

                void globalSend(IMessage* packet, const bool &reliable);

                /**
                 *  @brief Returns the current running status of the server.
                 *  @return A boolean representing whether or not the server is running.
                 */
                bool isRunning(void);

                //! Causes the server to handle all queued network events immediately.
                void dispatch(void);

                virtual void update(const Common::F32& deltaTimeSeconds);

                virtual IIncomingClient* onReceiveClientChallenge(RemoteHostContext client) = 0;

                /**
                 *  @brief Callback function that is called upon the server's underlaying
                 *  network subsystem accepting a remote host connection.
                 *  @param client A pointer to a Kiaro::Network::IncomingClientBase representing
                 *  the incoming connection.
                 */
                virtual void onClientConnected(IIncomingClient* client);

                /**
                 *  @brief Callback function that is called upon the server's underlaying
                 *  network subsystem disconnecting a remote host connection.
                 *  @param client A pointer to a Kiaro::Network::IncomingClientBase representing
                 *  the disconnected client.
                 */
                virtual void onClientDisconnected(Net::IIncomingClient* client);

                Net::IIncomingClient* getLastPacketSender(void);

                Common::U32 getClientCount(void);

               // Kiaro::Network::IncomingClientBase *GetLastPacketSender(void);
                clientIterator clientsBegin(void) { return mConnectedClientSet.begin(); }

                clientConstIterator clientsEnd(void) { return mConnectedClientSet.end(); }
                
                virtual void onReceivePacket(Support::CBitStream& in, Net::IIncomingClient* sender) = 0;

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

            // Private Methods
            protected:
                void processPacket(Support::CBitStream& incomingStream, Net::IIncomingClient* sender);
                virtual void processStageZero(const IMessage& header, Support::CBitStream& incomingStream, Net::IIncomingClient* sender) = 0;

            // Private Members
            protected:
                Net::IIncomingClient* mLastPacketSender;

                bool mIsRunning;

                ENetHost* mInternalHost;

                //! The Port number that we're listening on.
                const Common::U16 mListenPort;
                //! The Address that we're listening on.
                const Support::String mListenAddress;

                Common::U32 mMaximumClientCount;

                //! An unordered set of all clients that passed the authentication stage and are now technically playing.
                Support::UnorderedSet<IIncomingClient*> mConnectedClientSet;
                //! An unordered set of all clients waiting to pass the authentication stage.
                Support::UnorderedSet<IIncomingClient*> mPendingClientSet;
        };
    } // End Namespace Network
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_GAME_SERVERSINGLETON_HPP_
