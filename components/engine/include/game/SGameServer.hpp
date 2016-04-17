/**
 */

#ifndef _INCLUDE_GAME_SGAMESERVER_HPP_
#define _INCLUDE_GAME_SGAMESERVER_HPP_

#include <phys/CSimulation.hpp>

#include <net/IServer.hpp>

#include <support/SSynchronousScheduler.hpp>

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
                 *  @param game The game mode to use.
                 */
                void setGamemode(IGameMode* game);

                virtual void update(void);

                void onClientConnected(Net::IIncomingClient* client);

                Net::IIncomingClient* onReceiveClientChallenge(Net::RemoteHostContext client);

                void initialScope(Net::IIncomingClient* client);

            // Protected Methods
            protected:
                void onReceivePacket(Support::CBitStream& in, Net::IIncomingClient* sender);
                void processStageZero(const Net::IMessage& header, Support::CBitStream& incomingStream, Net::IIncomingClient* sender);

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
