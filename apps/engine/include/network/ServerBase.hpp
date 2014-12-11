/**
 *  @file ServerBase.hpp
 *  @brief Include file defining the Server and related functions.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_NETWORK_SERVER_H_
#define _INCLUDE_KIARO_NETWORK_SERVER_H_

#include <vector>

#include <enet/enet.h>

#include <network/IncomingClientBase.hpp>

#include <support/MapDivision.hpp>

#include "engine/Common.hpp"

namespace Kiaro
{
    namespace Game
    {
        class GamemodeBase;
    } // End NameSpace Game

    namespace Support
    {
        class BitStream;
        class MapDivision;
    } // End NameSpace Support

    namespace Network
    {
        class PacketBase;
        class IncomingClientBase;

        //! Server class that remote hosts connect to.
        class ServerBase
        {
            public:
            /**
             *  @brief Constructor that initializes a server instance with the given information.
             *  @param listen_address An array of c8 representing the IP address to listen on.
             *  @param listen_port A u16 representing the port number to listen on.
             *  @param clients A u32 representing the maximum number of clients to accept.
             */
            ServerBase(const std::string &listenAddress, const Kiaro::Common::U16 &listenPort, const Kiaro::Common::U32 &maximumClientCount);

            /**
			 *  @brief Standard virtual destructor.
			 *  @note The only reason this destructor is virtual is to prevent compiler warnings when deleting instances that
			 *  are children of this Server class.
			 */
            ~ServerBase(void);

            /**
             *  @brief Signals the server to stop running.
             */
            void stop(void);

            /**
             *  @brief Returns the current running status of the server.
             *  @return A boolean representing whether or not the server is running.
             */
            bool isRunning(void) { return mIsRunning; }

            Kiaro::Common::U32 getClientCount(void);

            void globalSend(Kiaro::Network::PacketBase *packet, const bool &reliable);

            void update(void);

            /**
             *  @brief Causes the server to handle all queued network events immediately.
             */
            void dispatch(void) { if (mIsRunning) enet_host_flush(mInternalHost); }

            void setGamemode(Kiaro::Game::GamemodeBase *game);

            //! Callback function that is called upon the server's underlaying network subsystem accepting a remote host connection.
            virtual void onClientConnected(Kiaro::Network::IncomingClientBase *client) = 0;
            //! Callback function that is called upon the disconnection of a remote host.
            virtual void onClientDisconnected(Kiaro::Network::IncomingClientBase *client) = 0;

            virtual void onReceivePacket(Kiaro::Support::BitStream &incomingStream, Kiaro::Network::IncomingClientBase *sender) = 0;

            Kiaro::Network::IncomingClientBase *getLastPacketSender(void);

            // Protected Members
			protected:
                Kiaro::Game::GamemodeBase *mCurrentGamemode;

                bool mIsRunning;

                ENetHost *mInternalHost;

                //! The Port number that we're listening on.
                const Kiaro::Common::U16 mListenPort;
                //! The Address that we're listening on.
                const std::string mListenAddress;

                std::set<size_t> mConnectedClientSet;
        };
    } // End Namespace Network
} // End Namespace Kiaro

#endif // _INCLUDE_KIARO_NETWORK_SERVER_H_
