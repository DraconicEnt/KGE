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

        class SGameServer : public Net::IServer
        {
            public:
                static void initialize(void);
                static SGameServer* getPointer(void);
                static void destroy(void);

                void setGamemode(IGameMode* game);

                virtual void update(void);
                
                void onClientConnected(Net::IIncomingClient* client);
                
                Net::IIncomingClient* onReceiveClientChallenge(Net::RemoteHostContext client);
                
                void initialScope(Net::IIncomingClient* client);
            
            protected:
                void onReceivePacket(Support::CBitStream& in, Net::IIncomingClient* sender);
                void processStageZero(const Net::IMessage& header, Support::CBitStream& incomingStream, Net::IIncomingClient* sender);

            private:
                SGameServer(const Support::String& listenAddress, const Common::U16& listenPort, const Common::U32& maximumClientCount);
                ~SGameServer(void);

                Game::IGameMode* mCurrentGamemode;
                Phys::CSimulation* mSimulation;
                
                Support::CScheduledEvent* mUpdatePulse;
        };
    } // End NameSpace Game
} // End NameSpace Kiaro
#endif // _INCLUDE_GAME_SGAMESERVER_HPP_