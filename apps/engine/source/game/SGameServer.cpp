/**
 */

#include <core/SSettingsRegistry.hpp>

#include <game/SGameServer.hpp>

#include <game/IGameMode.hpp>

#include <game/entities/entities.hpp>

namespace Kiaro
{
    namespace Game
    {
        static SGameServer* sInstance = NULL;

        SGameServer* SGameServer::getPointer(void)
        {
            if (!sInstance)
            {
                Core::SSettingsRegistry* settings = Core::SSettingsRegistry::getPointer();

                sInstance = new SGameServer(settings->getValue<Support::String>("ListenAddress"),
                                            settings->getValue<Common::U16>("ListenPort"),
                                            settings->getValue<Common::U32>("MaximumClientCount"));
            }

            return sInstance;
        }

        void SGameServer::destroy(void)
        {
            if (sInstance)
            {
                delete sInstance;
                sInstance = NULL;
            }
        }

        SGameServer::SGameServer(const Support::String& listenAddress, const Common::U16& listenPort, const Common::U32& maximumClientCount) : Net::IServer(listenAddress, listenPort, maximumClientCount)
        {
            // Create a terrain entity to test
            Game::Entities::CTerrain* terrain = new Game::Entities::CTerrain("textures/terrain-heightmap.bmp");
            Game::SGameWorld::getPointer()->addEntity(terrain);
        }

        SGameServer::~SGameServer(void)
        {
            // FIXME (Robert MacGregor#9): Call IServer destructor?
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

        void SGameServer::update(const Common::F32& deltaTimeSeconds)
        {
            Net::IServer::update(deltaTimeSeconds);

            //mCurrentGamemode->
        }
    } // End NameSpace Game
} // End NameSpace Kiaro
