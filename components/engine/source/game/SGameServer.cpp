/**
 */

#include <support/SSettingsRegistry.hpp>

#include <game/SGameServer.hpp>

#include <game/IGameMode.hpp>

#include <game/entities/entities.hpp>

#include <game/CGameClient.hpp>

namespace Kiaro
{
    namespace Game
    {
        static SGameServer* sInstance = nullptr;

        void SGameServer::initialize(void)
        {
            if (!sInstance)
            {
                Support::SSettingsRegistry* settings = Support::SSettingsRegistry::getPointer();

                sInstance = new SGameServer(settings->getValue<Support::String>("Server::ListenAddress"),
                                            settings->getValue<Common::U16>("Server::ListenPort"),
                                            settings->getValue<Common::U32>("Server::MaximumClientCount"));
            }
        }

        SGameServer* SGameServer::getPointer(void)
        {
            return sInstance;
        }

        void SGameServer::destroy(void)
        {
            if (sInstance)
            {
                delete sInstance;
                sInstance = nullptr;
            }
        }

        Net::IIncomingClient* SGameServer::onReceiveClientChallenge(Net::RemoteHostContext client)
        {
            CGameClient* incoming = new CGameClient(client);
            return incoming;
        }

        SGameServer::SGameServer(const Support::String& listenAddress, const Common::U16& listenPort, const Common::U32& maximumClientCount) : Net::IServer(listenAddress, listenPort, maximumClientCount)
        {
            mSimulation = new Phys::CSimulation();
            
            Entities::CTerrain* terrain = new Entities::CTerrain("textures/terrain-heightmap.bmp");
            
            // Add our update to the scheduler
            mUpdatePulse = Support::SSynchronousScheduler::getPointer()->schedule(32, true, this, &SGameServer::update);
        }

        SGameServer::~SGameServer(void)
        {
            // FIXME (Robert MacGregor#9): Call IServer destructor?
            delete mSimulation;
            mSimulation = nullptr;
        }

        void SGameServer::setGamemode(IGameMode* game)
        {
            if (mCurrentGamemode)
            {
                mCurrentGamemode->tearDown();
                delete mCurrentGamemode;
            }

            mCurrentGamemode = game;

            if (mCurrentGamemode)
                mCurrentGamemode->setup();
        }

        void SGameServer::update(void)
        {
            Net::IServer::update(0);

            //mCurrentGamemode->
        }
    } // End NameSpace Game
} // End NameSpace Kiaro
