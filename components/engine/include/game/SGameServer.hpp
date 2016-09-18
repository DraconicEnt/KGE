/**
 *  @file SGameWorld.hpp
 *  @brief Include file declaring the SGameServer singleton class and its methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_GAME_SGAMESERVER_HPP_
#define _INCLUDE_GAME_SGAMESERVER_HPP_

#include <phys/CSimulation.hpp>

#include <net/IServer.hpp>

#include <support/SSynchronousScheduler.hpp>
#include <support/UnorderedMap.hpp>

#include <net/stages.hpp>
#include <net/IMessage.hpp>

#include <game/entities/datablocks/IDataBlock.hpp>

namespace Kiaro
{
    namespace Game
    {
        class IGameMode;

        /**
         *  @brief Singleton class representing a running game server.
         */
        class SGameServer : public Net::IServer
        {
            // Private Members
            private:
                //! The currently running gamemode programming.
                Game::IGameMode* mCurrentGamemode;

                //! The physical simulation in use.
                Phys::CSimulation* mSimulation;

                //! Scheduled event created for use with the SSynchronousScheduler.
                Support::CScheduledEvent* mUpdatePulse;

                //! Any queued streams we still haven't processed for any given client.
                Support::UnorderedMap<Net::IIncomingClient*, Support::Queue<Support::CBitStream*>> mQueuedStreams;

                //! Vector of all datablocks available.
                Support::Set<Game::Entities::DataBlocks::IDataBlock*> mDataBlocks;

            // Public Methods
            public:
                /**
                 *  @brief Initializes and starts a new game server.
                 */
                static void initialize(void);

                /**
                 *  @brief Obtains a pointer to the running game server. If there isn't one,
                 *  nullptr is returned.
                 */
                static SGameServer* getPointer(void);

                /**
                 *  @brief Destroys the running game server if there is one.
                 */
                static void destroy(void);

                /**
                 *  @brief Sets the running gamemode of this server.
                 *  @param game The game mode to use. If null, then no gamemode programming will be
                 *  in use.
                 */
                void setGamemode(IGameMode* game);

                virtual void update(void);

                /**
                 *  @brief Called when the client passes the initial authentication stages.
                 *  @param client The client that has passed the authentication process.
                 */
                void onClientConnected(Net::IIncomingClient* client);

                /**
                 *  @brief Callback function that is called upon the server's underlaying
                 *  network subsystem disconnecting a remote host connection.
                 *  @param client A pointer to a Kiaro::Network::IncomingClientBase representing
                 *  the disconnected client.
                 */
                virtual void onClientDisconnected(Net::IIncomingClient* client);

                /**
                 *  @brief Called by the network component when a client first initiates a connection to the
                 *  running game server.
                 *  @param client The internal remote host context object that the network system created.
                 *  Just pass this back into the appropriate network subsystem constructor.
                 *  @return The newly created incoming client context.
                 */
                Net::IIncomingClient* onReceiveClientChallenge(Net::RemoteHostContext client);

                /**
                 *  @brief Produces a packet or series of packets whose purpose is to perform
                 *  initial server state networking to synchronize basic elements such as advertising
                 *  the existing of always-scoped objects such as the terrain.
                 *  @details This method should generally only be used when a client first connects and
                 *  passes the authentication stages for the first time or when a new map is loaded
                 *  and clients need to be informed about the basic details of the currently loaded map.
                 *
                 *  The payload generated will include non-scoped and static objects and does not bother
                 *  with trying to
                 *  @param client The connected client to network this information to.
                 */
                void initialScope(Net::IIncomingClient* client);

                void handshakeHandler(Net::IIncomingClient* sender, Support::CBitStream& in);

                bool addDataBlock(Game::Entities::DataBlocks::IDataBlock* datablock);

            // Protected Methods
            protected:
                void onReceivePacket(Support::CBitStream& in, Net::IIncomingClient* sender);

            // Private Methods
            private:
                /**
                 *  @brief Constructor accepting a listen address, port & maximum client count.
                 *  @param listenAddress The IP address to listen on.
                 *  @param listenPort The port number to listen in.
                 *  @param maximumClientCount The maximum number of clients to allow into the running game server.
                 */
                SGameServer(const Support::String& listenAddress, const Common::U16& listenPort, const Common::U32& maximumClientCount);

                //! Standard destructor.
                ~SGameServer(void);
        };
    } // End NameSpace Game
} // End NameSpace Kiaro
#endif // _INCLUDE_GAME_SGAMESERVER_HPP_
