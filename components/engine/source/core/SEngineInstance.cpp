/**
 *  @file SEngineInstance.cpp
 *  @brief Source code associated with the Core::SEngineInstance singleton class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2015 Draconic Entity
 */

#include <core/SEngineInstance.hpp>

#include <video/SRenderer.hpp>

#include <irrlicht.h>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Irrlicht/Renderer.h>

#if defined(ENGINE_UNIX)
    #include <allegro5/allegro_x.h>
#elif defined(ENGINE_WIN)
    #include <allegro5/allegro_windows.h>
#else
    #include <allegro5/allegro_osx.h>
#endif

#include <core/config.hpp>
#include <support/tasking/SAsynchronousTaskManager.hpp>
#include <support/tasking/SAsynchronousSchedulerTask.hpp>
#include <filesystem/SResourceProvider.hpp>
#include <input/SInputListener.hpp>
#include <support/SSettingsRegistry.hpp>

#include <game/SGameServer.hpp>

#include <net/IOutgoingClient.hpp>
#include <net/IIncomingClient.hpp>

#include <support/FTime.hpp>
#include <support/SSynchronousScheduler.hpp>

#include <video/CSceneGraph.hpp>

#include <support/Console.hpp>

#include <core/COutgoingClient.hpp>

namespace Kiaro
{
    namespace Core
    {
        static SEngineInstance* sInstance = nullptr;

        SEngineInstance* SEngineInstance::getPointer(void)
        {
            if (!sInstance)
                sInstance = new SEngineInstance;

            return sInstance;
        }

        void SEngineInstance::destroy(void)
        {
            if (sInstance)
            {
                sInstance->kill();
                delete sInstance;
            }

            sInstance = nullptr;
        }

        void SEngineInstance::setMode(const MODE_NAME& mode)
        {
            mEngineMode = mode;
        }

        void SEngineInstance::setTargetServer(Common::C8* address, Common::U16 port)
        {
            mTargetServerAddress = address;
            mTargetServerPort = port;
        }

        void SEngineInstance::setGame(const Support::String& gameName)
        {
            mGameName = gameName;
        }

        bool SEngineInstance::isDedicated(void)
        {
            return mEngineMode & Kiaro::Core::MODE_DEDICATED;
        }

        Kiaro::Common::S32 SEngineInstance::start(const Common::S32& argc, Common::C8* argv[])
        {
            mRunning = true;

            al_init();
            al_install_mouse();

            this->initializeFileSystem(argc, argv);

            Support::Console::writef(Support::Console::MESSAGE_INFO, "SEngineInstance: Running game '%s'", mGameName.data());

            // Add the game search path
            if (PHYSFS_mount(mGameName.c_str(), nullptr, 1) == 0)
            {
                mRunning = false;

                Support::Console::writef(Support::Console::MESSAGE_FATAL, "SEngineInstance: Failed to mount game directory '%s'. Reason: '%s'", mGameName.data(), PHYSFS_getLastError());
                return -1;
            }
            else
                Support::Console::writef(Support::Console::MESSAGE_INFO, "SEngineInstance: Mounted game directory '%s' successfully.", mGameName.data());

            Support::SSettingsRegistry* settings = Support::SSettingsRegistry::getPointer();

            // TODO (Robert MacGregor#9): Return error codes for the netcode

            // Init the taskers
            Support::SSynchronousScheduler* syncScheduler = Support::SSynchronousScheduler::getPointer();
            Support::Tasking::SAsynchronousTaskManager* asyncTaskManager = Support::Tasking::SAsynchronousTaskManager::getPointer();
            asyncTaskManager->addTask(Support::Tasking::SAsynchronousSchedulerTask::getPointer());

            mRunning = this->initializeRenderer() == 0 ? true : false;
            if (!mRunning)
                return 2;

            mRunning = this->initializeGUI() == 0 ? true : false;
            if (!mRunning)
                return 3;

            // Only init sound if we're not a dedicated server.
            if (mEngineMode != Kiaro::Core::MODE_DEDICATED)
                this->initializeSound();

            this->initializeNetwork();

            // Initialize the time pulses
            this->initializeScheduledEvents();

            this->runGameLoop();

            return 1;
        }

        void SEngineInstance::kill(void)
        {
            if (!mRunning)
                return;

            Support::Console::write(Support::Console::MESSAGE_INFO, "SEngineInstance: Killed via kill()");
            mRunning = false;
        }

        SEngineInstance::SEngineInstance(void) : mEngineMode(MODE_CLIENT), mTargetServerAddress("127.0.0.1"), mTargetServerPort(11595),
        mRunning(false), mActiveClient(nullptr)
        {

        }

        SEngineInstance::~SEngineInstance(void)
        {
            Support::Console::write(Support::Console::MESSAGE_INFO, "SEngineInstance: Deinitializing ...");

            // TODO: Check the destroy order
         //   Net::SClient::destroy();
            Game::SGameServer::destroy();
            Input::SInputListener::destroy();
            Support::SSynchronousScheduler::destroy();

            Support::SSettingsRegistry::destroy();

            PHYSFS_deinit();
            enet_deinitialize();

            Video::SRenderer::destroy();

            al_uninstall_system();
        }

        void SEngineInstance::networkUpdate(void)
        {
            // FIXME (Robert MacGregor#9): Pass in the time delta
  //          Net::SClient* client = Net::SClient::getPointer();
           // if (mActiveClient)
           //     mActiveClient->update();

            Game::SGameServer* server = Game::SGameServer::getPointer();
           // if (server)
           //     server->update(0.0f);
        }

        int SEngineInstance::initializeGUI(void)
        {
            return 0;
        }

        Common::U32 SEngineInstance::initializeRenderer(void)
        {
            Video::SRenderer* renderer = Video::SRenderer::getPointer();
            return 0;
        }

        Common::U32 SEngineInstance::initializeNetwork(void)
        {
            // Catch if the netcode somehow doesn't initialize correctly.
            if (enet_initialize() < 0)
            {
                Support::Console::write(Support::Console::MESSAGE_FATAL, "SEngineInstance: Failed to initialize the network!");
                return 1;
            }

            // Initialize the client or server ends
            switch (mEngineMode)
            {
                case Core::MODE_CLIENT:
                {
                    mActiveClient = new COutgoingClient();
                    break;
                }

                case Core::MODE_CLIENTCONNECT:
                {
                    mActiveClient = new COutgoingClient();
                    mActiveClient->connect(mTargetServerAddress, mTargetServerPort, 5000);

                    // FIXME (Robert MacGregor#9): Move error check elsewhere since the connection isn't acknowledged until the handshakes occur
                   // if (!mActiveClient->isConnected())
                   //     Support::Logging::write(Support::Logging::MESSAGE_FATAL, "SEngineInstance: Failed to connect to remote host with server flag!");

                    break;
                }

                case Core::MODE_DEDICATED:
                {
                    Game::SGameServer::initialize();
                    break;
                }
            }

            Support::Console::write(Support::Console::MESSAGE_INFO, "SEngineInstance: Initialized network.");
            return 0;
        }

        void SEngineInstance::runGameLoop(void)
        {
            // Start the Loop
            Common::F32 deltaTimeSeconds = 0.0f;

            while (mRunning)
            {
                try
                {
                    // Update all our subsystems
                    Support::FTime::timer timerID = Support::FTime::startTimer();
                    Support::Tasking::SAsynchronousTaskManager::getPointer()->tick();

                    // Pump a time pulse at the scheduler
                    Support::SSynchronousScheduler::getPointer()->update();

                    // The GUI, video and sound systems run independently of our network time pulse
                    if (mEngineMode == Core::MODE_CLIENT || mEngineMode == Core::MODE_CLIENTCONNECT)
                        CEGUI::System::getSingleton().injectTimePulse(deltaTimeSeconds);

                    deltaTimeSeconds = Support::FTime::stopTimer(timerID);
                }
                catch(std::exception& e)
                {
                    Support::Console::writef(Support::Console::MESSAGE_ERROR, "SEngineInstance: An internal exception of type '%s' has occurred:\n%s", typeid(e).name(), e.what());

                    // Something is probably up, we should leave if we have an active client.
                    if (mActiveClient)
                        mActiveClient->disconnect();

                    // Servers just drop off the client that it last processed
                    Game::SGameServer* server = Game::SGameServer::getPointer();
                    if (server)
                    {
                        Net::IIncomingClient* lastClient = server->getLastPacketSender();

                        // TODO (Robert MacGregor#9): Handle for if an exception is thrown AND we are running as a listen server.
                        if (lastClient)
                            lastClient->disconnect("Internal Exception");
                    }
                }
            }
        }

        Common::U32 SEngineInstance::initializeSound(void)
        {
            #ifndef ENGINE_BUILD_SOUNDENGINE
                Support::Console::write(Support::Console::MESSAGE_WARNING, "SEngineInstance: Built without audio support. There will be no sound.");
            #else
            #endif // ENGINE_BUILD_SOUNDENGINE

            return 0;
        }

        void SEngineInstance::initializeFileSystem(const Common::S32& argc, Common::C8* argv[])
        {
            // Initialize the file system
            PHYSFS_init(argv[0]);
            PHYSFS_setSaneConfig("Draconic Entertainment", "KGE", "ZIP", 0, 0);

            // Remove the search path that points to the same directory as the executable
            // TODO (Robert MacGregor#9): Research this.
            Common::C8** searchPaths = PHYSFS_getSearchPath();
            Common::C8* searchPath = searchPaths[1];
            PHYSFS_removeFromSearchPath(searchPath);
            PHYSFS_freeList(searchPaths);

            // TODO (Robert MacGregor#9): Initialize Allegro with PhysFS
        }

        void SEngineInstance::initializeScheduledEvents(void)
        {
            Support::SSynchronousScheduler* syncScheduler = Support::SSynchronousScheduler::getPointer();

            // Clients get a handful of scheduled events that don't apply for dedicated servers
            if (mEngineMode != MODE_DEDICATED)
            {
                Input::SInputListener* inputListener = Input::SInputListener::getPointer();

                // Set up input sampling
                syncScheduler->schedule(Support::FPSToMS(75.0f), true, inputListener, &Input::SInputListener::update);
            }

            syncScheduler->schedule(ENGINE_TICKRATE, true, this, &SEngineInstance::networkUpdate);
        }
    } // End Namespace Engine
} // End Namespace Kiaro
