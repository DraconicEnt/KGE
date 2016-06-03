/**
 */

#ifndef _INCLUDE_GAME_SGAMESERVER_HPP_
#define _INCLUDE_GAME_SGAMESERVER_HPP_

#include <phys/CSimulation.hpp>

#include <net/IServer.hpp>

#include <support/SSynchronousScheduler.hpp>

#include <net/stages.hpp>

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


                // Initialize a net test
                typedef EasyDelegate::DelegateSet<Net::IMessage*, Support::CBitStream&> TestSet;
                typedef EasyDelegate::DelegateSet<void, Net::IIncomingClient*, Support::CBitStream&> MessageHandlerSet;

                Common::U32 mMessageCounter;
                Support::UnorderedMap<Common::U8, Support::UnorderedMap<Common::U32, std::pair<TestSet::StaticDelegateFuncPtr, MessageHandlerSet::MemberDelegateFuncPtr<SGameServer>>>> mStageMap;
                Support::UnorderedMap<Common::U32, TestSet::StaticDelegateFuncPtr> mMessageMap;

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

                void registerMessage(TestSet::StaticDelegateFuncPtr messageConstructor, MessageHandlerSet::MemberDelegateFuncPtr<SGameServer> handler, const Net::STAGE_NAME stage);


                void handshakeHandler(Net::IIncomingClient* sender, Support::CBitStream& in);

                //! Standard destructor.
                ~SGameServer(void);
        };
    } // End NameSpace Game
} // End NameSpace Kiaro
#endif // _INCLUDE_GAME_SGAMESERVER_HPP_
