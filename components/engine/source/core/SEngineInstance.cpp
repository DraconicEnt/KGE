/**
 *  @file SEngineInstance.cpp
 *  @brief Source code associated with the Core::SEngineInstance singleton class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

// FIXME: Caused include issues with Python.h
#include <chrono>
#include <thread>

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

#include <allegro5/allegro_physfs.h>

#include <support/tasking/SAsynchronousTaskManager.hpp>
#include <input/SInputListener.hpp>
#include <support/SSettingsRegistry.hpp>

#include <core/config.hpp>
#include <net/IIncomingClient.hpp>

#include <video/CSceneGraph.hpp>

#include <support/SProfiler.hpp>

#include <sound/SSoundManager.hpp>

#include <game/messages/messages.hpp>
#include <gui/SGUIManager.hpp>

#include <core/SCoreRegistry.hpp>
#include <support/FCPUInfo.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Core
        {
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
                return mEngineMode & MODE_DEDICATED;
            }

            Kiaro::Common::S32 SEngineInstance::start(const Common::S32 argc, Common::C8* argv[])
            {
                // Before doing anything, read CPU data
                Support::getCPUInformation();

                mRunning = false;
                al_init();
                this->initializeFileSystem(argc, argv);

                // Once the filesystem is initialized, pump Allegro through it
                al_set_physfs_file_interface();
                CONSOLE_INFOF("Running game '%s'", mGameName.data());

                // Mount the data directories
                Support::Vector<Support::String> mountedDirectories = mModNames;
                mountedDirectories.push_back(mGameName);

                for (Support::String directory: mountedDirectories)
                {
                    if (PHYSFS_mount(directory.c_str(), nullptr, 1) == 0)
                    {
                        mRunning = false;
                        CONSOLE_ERRORF("'%s'. Reason: '%s'", directory.c_str(), PHYSFS_getLastError());
                        return -1;
                    }

                    CONSOLE_INFOF("Mounted game directory '%s' successfully.", directory.c_str());
                }

                // TODO (Robert MacGregor#9): Return error codes for the netcode
                // Init the taskers
                Support::SSynchronousScheduler* syncScheduler = Support::SSynchronousScheduler::getInstance();
                Support::Tasking::SAsynchronousTaskManager* asyncTaskManager = Support::Tasking::SAsynchronousTaskManager::getInstance();

                if (this->initializeRenderer() != 0)
                    return 2;

                if (!this->isDedicated() && this->initializeGUI() != 0)
                    return 3;

                // Only init sound if we're not a dedicated server.
                if (mEngineMode != MODE_DEDICATED)
                    this->initializeSound();

                this->initializeNetwork();

                // Initialize the time pulses
                this->initializeScheduledEvents();

                mRunning = true;
                this->runGameLoop();
                return 1;
            }

            void SEngineInstance::setMods(const Support::Vector<Support::String>& mods)
            {
                mModNames = mods;
            }

            void SEngineInstance::kill(void)
            {
                if (!mRunning)
                    return;

                CONSOLE_INFO("Killed via kill()");
                mRunning = false;
            }

            SEngineInstance::SEngineInstance(void) : mEngineMode(MODE_CLIENT), mTargetServerAddress("127.0.0.1"), mTargetServerPort(11595),
            mRunning(false), mActiveClient(nullptr), mPerfStatSchedule(nullptr)
            {
            }

            SEngineInstance::~SEngineInstance(void)
            {
                CONSOLE_INFO("Deinitializing ...");

                if (mPerfStatSchedule)
                    mPerfStatSchedule->cancel();

                mPerfStatSchedule = nullptr;
                // TODO: Check the destroy order
                //   Net::SClient::destroy();

                Game::SGameServer::destroy();
                Input::SInputListener::destroy();
                Support::SSynchronousScheduler::destroy();
                Support::SSettingsRegistry::destroy();

                PHYSFS_deinit();
                enet_deinitialize();

                Engine::Video::SRenderer::destroy();
                al_uninstall_system();
            }

            int SEngineInstance::initializeGUI(void)
            {
                Engine::GUI::SGUIManager::getInstance();

                return 0;
            }

            Common::U32 SEngineInstance::initializeRenderer(void)
            {
                Engine::Video::SRenderer::getInstance();

                return 0;
            }

            Common::U32 SEngineInstance::initializeNetwork(void)
            {
                // Catch if the netcode somehow doesn't initialize correctly.
                if (enet_initialize() < 0)
                {
                    CONSOLE_ERROR("Failed to initialize the network!");
                    return 1;
                }

                SCoreRegistry* registry = SCoreRegistry::getInstance();

                // Initialize the client or server ends
                switch (mEngineMode)
                {
                    case MODE_CLIENT:
                    {
                        mActiveClient = new COutgoingClient();
                        break;
                    }

                    case MODE_CLIENTCONNECT:
                    {
                        mActiveClient = new COutgoingClient();
                        mActiveClient->connect(mTargetServerAddress, mTargetServerPort, 5000);
                        // FIXME (Robert MacGregor#9): Move error check elsewhere since the connection isn't acknowledged until the handshakes occur
                        // if (!mActiveClient->isConnected())
                        //     Support::Logging::write(Support::Logging::MESSAGE_FATAL, "SEngineInstance: Failed to connect to remote host with server flag!");
                        break;
                    }

                    case MODE_DEDICATED:
                    {
                        Game::SGameServer::initialize();
                        break;
                    }
                }

                CONSOLE_INFO("Initialized network.");
                return 0;
            }

            void SEngineInstance::runGameLoop(void)
            {
                // Start the Loop
                Common::F32 deltaTimeSeconds = 0.0f;

                while (mRunning)
                {
                    #if _ENGINE_USE_GLOBAL_EXCEPTION_CATCH_ > 0
                    try
                    {
                    #endif

                        // Update all our subsystems
                        Support::FTime::timer timerID = Support::FTime::startTimer();
                        PROFILER_BEGIN(MainLoop);
                        std::this_thread::sleep_for(std::chrono::nanoseconds(500000));
                        Support::Tasking::SAsynchronousTaskManager::getInstance()->tick();

                        // Pump a time pulse at the scheduler
                        Support::SSynchronousScheduler::getInstance()->update();

                        // The GUI, video and sound systems run independently of our network time pulse
                        if (mEngineMode == MODE_CLIENT || mEngineMode == MODE_CLIENTCONNECT)
                        {
                            CEGUI::System::getSingleton().injectTimePulse(deltaTimeSeconds);
                            Sound::SSoundManager::getInstance()->update();
                        }

                        PROFILER_END(MainLoop);
                        deltaTimeSeconds = Support::FTime::stopTimer(timerID);
                    #if _ENGINE_USE_GLOBAL_EXCEPTION_CATCH_ > 0
                    }
                    catch(std::exception& e)
                    {
                        CONSOLE_ERRORF("An internal exception of type '%s' has occurred:\n%s", typeid(e).name(), e.what());

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

                    #endif
                }
            }

            Common::U32 SEngineInstance::initializeSound(void)
            {
                Sound::SSoundManager::getInstance();
                return 0;
            }

            void SEngineInstance::initializeFileSystem(const Common::S32 argc, Common::C8* argv[])
            {
                // Initialize the file system
                PHYSFS_init(argv[0]);
                PHYSFS_setSaneConfig("Draconic Entity", "KGE", "ZIP", 0, 0);

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
                Support::SSynchronousScheduler* syncScheduler = Support::SSynchronousScheduler::getInstance();

                // Clients get a handful of scheduled events that don't apply for dedicated servers
                if (mEngineMode != MODE_DEDICATED)
                {
                    Input::SInputListener* inputListener = Input::SInputListener::getInstance();

                    // Set up input sampling
                    syncScheduler->schedule(Support::FPSToMS(75.0f), true, inputListener, &Input::SInputListener::update);
                }
            }

            void SEngineInstance::setPerfStatEnabled(const bool enabled)
            {
                Support::SSynchronousScheduler* syncScheduler = Support::SSynchronousScheduler::getInstance();

                if (mPerfStatSchedule && enabled)
                    return;
                else if (!mPerfStatSchedule && enabled)
                {
                    mPerfStatSchedule = syncScheduler->schedule(4000, true, this, &SEngineInstance::printPerfStat);
                    CONSOLE_INFO("Enabled perf stat.");
                }
                else if (mPerfStatSchedule && !enabled)
                {
                    mPerfStatSchedule->cancel();
                    mPerfStatSchedule = nullptr;
                    CONSOLE_INFO("Disabled perf stat.");
                }
            }

            void SEngineInstance::printPerfStat(void)
            {
                Support::SProfiler* profiler = Support::SProfiler::getPointer();
                auto averages = profiler->getSampleAverages();
                CONSOLE_INFO("Performance Statistics---------------------------");

                for (auto average: averages)
                    CONSOLE_INFOF("%s: %f sec", average.first.data(), average.second);
            }
        } // End Namespace Engine
    }
} // End Namespace Kiaro
