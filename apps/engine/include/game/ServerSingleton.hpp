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

#include "engine/Common.hpp"

#include <network/ServerBase.hpp>

#include <game/entities/Entities.hpp>

namespace Kiaro
{
    namespace Engine
    {
        class EntityGroupingSingleton;
    }

    namespace Game
    {
        class GamemodeBase;

        namespace Entities
        {
            class EntityBase;
        }

        //! Server class that remote hosts connect to.
        class ServerSingleton : public Kiaro::Network::ServerBase
        {
            // Public Methods
            public:
                static ServerSingleton *getPointer(const Kiaro::Common::String &listenAddress = "0.0.0.0", const Kiaro::Common::U16 &listenPort = 11595, const Kiaro::Common::U32 &maximumClientCount = 32);
                static void destroy(void);

                //! Signals the server to stop running.
                void stop(void);

                /**
                 *  @brief Returns the current running status of the server.
                 *  @return A boolean representing whether or not the server is running.
                 */
                bool isRunning(void);

                //! Performs an update time pulse on the server.
                virtual void update(const Kiaro::Common::F32 &deltaTimeSeconds);

                //! Causes the server to handle all queued network events immediately.
                void dispatch(void);


                /**
                 *  @brief Callback function that is called upon the server's underlaying
                 *  network subsystem accepting a remote host connection.
                 *  @param client A pointer to a Kiaro::Network::IncomingClientBase representing
                 *  the incoming connection.
                 */
                void onClientConnected(Kiaro::Network::IncomingClientBase *client);

                /**
                 *  @brief Callback function that is called upon the server's underlaying
                 *  network subsystem disconnecting a remote host connection.
                 *  @param client A pointer to a Kiaro::Network::IncomingClientBase representing
                 *  the disconnected client.
                 */
                void onClientDisconnected(Kiaro::Network::IncomingClientBase *client);

                /**
                 *  @brief Callback function that is called upon the server's underlaying
                 *  network subsystem receiving a packet.
                 *  @param incomingStream A reference to the Kiaro::Support::BitStream that can be
                 *  used to unpack the packet payload.
                 *  @param sender A pointer to a Kiaro::Network::IncomingClientBase representing
                 *  the sender of the packet.
                 */
                void onReceivePacket(Kiaro::Support::BitStream &incomingStream, Kiaro::Network::IncomingClientBase *sender);

                Kiaro::Network::IncomingClientBase *getLastPacketSender(void);

                Kiaro::Common::U32 getClientCount(void);

               // Kiaro::Network::IncomingClientBase *GetLastPacketSender(void);

            // Private Methods
            private:
                /**
                 *  @brief Constructor that initializes a server instance with the given information.
                 *  @param listen_address An array of c8 representing the IP address to listen on.
                 *  @param listen_port A Kiaro::Common::U16 representing the port number to listen on.
                 *  @param clients A Kiaro::Common::U32 representing the maximum number of clients to accept.
                 */
                ServerSingleton(const Kiaro::Common::String &listenAddress, const Common::U16 &listenPort, const Common::U32 &maximumClientCount);

                /**
                 *  @brief Standard virtual destructor.
                 *  @note The only reason this destructor is virtual is to prevent compiler warnings when deleting instances that
                 *  are children of this Server class.
                 */
                ~ServerSingleton(void);

            // Private Members
            private:
                Kiaro::Network::IncomingClientBase *mLastPacketSender;

                Kiaro::Engine::EntityGroupingSingleton *mEntityGroup;
        };
    } // End Namespace Network
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_GAME_SERVERSINGLETON_HPP_
