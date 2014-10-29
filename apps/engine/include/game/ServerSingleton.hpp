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
    namespace Game
    {
        namespace Entities
        {
            class EntityBase;
        }

        //! Server class that remote hosts connect to.
        class ServerSingleton : public Kiaro::Network::ServerBase
        {
            // Public Methods
            public:
                static ServerSingleton *getPointer(const std::string &listenAddress = "0.0.0.0", const Kiaro::Common::U16 &listenPort = 11595, const Kiaro::Common::U32 &maximumClientCount = 32);
                static void destroy(void);

                /**
                 *  @brief Signals the server to stop running.
                 */
                void stop(void);

                /**
                 *  @brief Returns the current running status of the server.
                 *  @return A boolean representing whether or not the server is running.
                 */
                bool isRunning(void);

                void update(void);

                /**
                 *  @brief Causes the server to handle all queued network events immediately.
                 */
                void dispatch(void);

                //! Callback function that is called upon the server's underlaying network subsystem accepting a remote host connection.
                void onClientConnected(Kiaro::Network::IncomingClientBase *client);
                //! Callback function that is called upon the disconnection of a remote host.
                void onClientDisconnected(Kiaro::Network::IncomingClientBase *client);

                void onReceivePacket(Kiaro::Support::BitStream &incomingStream, Kiaro::Network::IncomingClientBase *sender);

                Kiaro::Network::IncomingClientBase *getLastPacketSender(void);

                Kiaro::Common::U32 getClientCount(void);

               // Kiaro::Network::IncomingClientBase *GetLastPacketSender(void);

                void addStaticEntity(Kiaro::Game::Entities::EntityBase *entity);

            // Private Methods
            private:
                /**
                 *  @brief Constructor that initializes a server instance with the given information.
                 *  @param listen_address An array of c8 representing the IP address to listen on.
                 *  @param listen_port A u16 representing the port number to listen on.
                 *  @param clients A u32 representing the maximum number of clients to accept.
                 */
                ServerSingleton(const std::string &listenAddress, const Common::U16 &listenPort, const Common::U32 &maximumClientCount);

                /**
                 *  @brief Standard virtual destructor.
                 *  @note The only reason this destructor is virtual is to prevent compiler warnings when deleting instances that
                 *  are children of this Server class.
                 */
                ~ServerSingleton(void);

            // Private Members
            private:
                Kiaro::Network::IncomingClientBase *mLastPacketSender;

                std::set<Kiaro::Game::Entities::EntityBase *> mStaticEntitySet;
                std::set<Kiaro::Game::Entities::EntityBase *> mDynamicEntitySet;
        };
    } // End Namespace Network
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_GAME_SERVERSINGLETON_HPP_
