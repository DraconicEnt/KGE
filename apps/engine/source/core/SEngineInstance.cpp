/**
 *  @file SEngineInstance.cpp
 *  @brief Source code associated with the Core::SEngineInstance singleton class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <core/SEngineInstance.hpp>

#include <irrlicht.h>
#include <lua.hpp>

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
#include <core/tasking/SAsynchronousTaskManager.hpp>
#include <core/tasking/SAsynchronousSchedulerTask.hpp>
#include <filesystem/SResourceProvider.hpp>
#include <input/SInputListener.hpp>
#include <support/SSettingsRegistry.hpp>

#include <game/SGameServer.hpp>

#include <net/IOutgoingClient.hpp>
#include <net/CIncomingClient.hpp>

#include <support/FTime.hpp>
#include <support/SSynchronousScheduler.hpp>

#include <video/CSceneGraph.hpp>

#include <support/Console.hpp>

#include <core/COutgoingClient.hpp>

namespace Kiaro
{
    namespace Core
    {
        static SEngineInstance* sInstance = NULL;

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

            sInstance = NULL;
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

        irr::IrrlichtDevice* SEngineInstance::getIrrlichtDevice(void)
        {
            return mIrrlichtDevice;
        }

        irr::scene::ISceneManager* SEngineInstance::getSceneManager(void)
        {
            return mSceneManager;
        }

        Kiaro::Common::S32 SEngineInstance::start(const Common::S32& argc, Common::C8* argv[])
        {
            mRunning = true;

            al_init();
            al_install_mouse();

            this->initializeFileSystem(argc, argv);

            Support::Console::write(Support::Console::MESSAGE_INFO, "SEngineInstance: Running game '%s'", mGameName.data());

            // Add the game search path
            if (PHYSFS_mount(mGameName.c_str(), NULL, 1) == 0)
            {
                mRunning = false;

                Support::Console::write(Support::Console::MESSAGE_FATAL, "SEngineInstance: Failed to mount game directory '%s'. Reason: '%s'", mGameName.data(), PHYSFS_getLastError());
                return -1;
            }
            else
                Support::Console::write(Support::Console::MESSAGE_INFO, "SEngineInstance: Mounted game directory '%s' successfully.", mGameName.data());

            Support::SSettingsRegistry* settings = Support::SSettingsRegistry::getPointer();

            // TODO (Robert MacGregor#9): Return error codes for Lua, renderer, GUI and the netcode
            mRunning = this->initializeLua(argc, argv) == 0 ? true : false;
            if (!mRunning)
                return 1;

            // Init the taskers
            Support::SSynchronousScheduler* syncScheduler = Support::SSynchronousScheduler::getPointer();
            Core::Tasking::SAsynchronousTaskManager* asyncTaskManager = Core::Tasking::SAsynchronousTaskManager::getPointer();
            asyncTaskManager->addTask(Core::Tasking::SAsynchronousSchedulerTask::getPointer());

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

        SEngineInstance::SEngineInstance(void) : mEngineMode(MODE_CLIENT), mIrrlichtDevice(NULL), mTargetServerAddress("127.0.0.1"), mTargetServerPort(11595),
        mRunning(false),// mClearColor(Kiaro::Common::ColorRGBA(255, 255, 0, 0)),
        mLuaState(NULL), mCurrentScene(NULL), mActiveClient(NULL), mDisplay(NULL)
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

            if (mIrrlichtDevice)
            {
                mIrrlichtDevice->drop();
                mIrrlichtDevice = NULL;
            }

            if (mLuaState)
            {
                lua_close(mLuaState);
                mLuaState = NULL;
            }

            Support::SSettingsRegistry::destroy();

            PHYSFS_deinit();
            enet_deinitialize();

            if (mDisplay)
            {
                al_destroy_display(mDisplay);
                mDisplay = NULL;
            }

            al_uninstall_system();
        }

        void SEngineInstance::networkUpdate(void)
        {
            // FIXME (Robert MacGregor#9): Pass in the time delta
  //          Net::SClient* client = Net::SClient::getPointer();
            if (mActiveClient)
                mActiveClient->update();

            Game::SGameServer* server = Game::SGameServer::getPointer();
            if (server)
                server->update(0.0f);
        }

        lua_State *SEngineInstance::getLuaState(void) { return mLuaState; }

        int SEngineInstance::initializeGUI(void)
        {
            // Start up CEGUI (if we're a client)
            if (mEngineMode == MODE_CLIENTCONNECT || mEngineMode == MODE_CLIENT)
            {
                try
                {
                    // We don't need the OS cursor
                    mIrrlichtDevice->getCursorControl()->setVisible(false);

                    CEGUI::IrrlichtRenderer& renderer = CEGUI::IrrlichtRenderer::create(*mIrrlichtDevice);
                    FileSystem::SResourceProvider *resourceProvider = FileSystem::SResourceProvider::getPointer();
                    CEGUI::System::create(renderer, static_cast<CEGUI::ResourceProvider*>(resourceProvider), NULL, NULL, NULL, "", "log.txt");

                    resourceProvider->setResourceGroupDirectory("fonts", "fonts/");
                    resourceProvider->setResourceGroupDirectory("ui", "ui/");

                    CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme", "ui");
                    CEGUI::FontManager::getSingleton().createFromFile( "DejaVuSans-10.font", "fonts" );

                    // Set the defaults
                    CEGUI::GUIContext& guiContext = CEGUI::System::getSingleton().getDefaultGUIContext();

                    guiContext.setDefaultFont( "DejaVuSans-10" );
                    guiContext.getMouseCursor().setDefaultImage( "TaharezLook/MouseArrow" );
                    guiContext.getMouseCursor().setImage(guiContext.getMouseCursor().getDefaultImage());

                    Support::Console::write(Support::Console::MESSAGE_INFO, "SEngineInstance: Initialized the GUI system.");
                }
                catch (CEGUI::InvalidRequestException& e)
                {
                    Support::Console::write(Support::Console::MESSAGE_FATAL, "SEngineInstance: Failed to initialize the GUI System. Reason:\n%s", e.what());
                    return 1;
                }
            }

            return 0;
        }

        Common::U32 SEngineInstance::initializeLua(const Common::S32& argc, Common::C8* argv[])
        {
            // Initialize Lua
            // FIXME (Robert MacGregor#9): Init and call our Lua main method within the try, catch
            mLuaState = luaL_newstate();
            luaL_checkversion(mLuaState);
            lua_gc(mLuaState, LUA_GCSTOP, 0);
            luaL_openlibs(mLuaState);
            lua_gc(mLuaState, LUA_GCRESTART, 0);

            // Now create the callback tables
            lua_createtable(mLuaState, 0, 0);
            lua_setglobal(mLuaState, "GameServer");
            lua_createtable(mLuaState, 0, 0);
            lua_setglobal(mLuaState, "GameClient");

            // The game window is used for callbacks from the window
            lua_createtable(mLuaState, 0, 0);
            lua_setglobal(mLuaState, "GameWindow");

            // Load up the main file
            if (luaL_dofile(mLuaState, "main.lua") >= 1)
            {
                Support::Console::write(Support::Console::MESSAGE_FATAL, "SEngineInstance.cpp: Failed to load main.lua. Reason: '%s'", luaL_checkstring(mLuaState, -1));
                return 2;
            }

            // Call the main(argv) method
            lua_getglobal(mLuaState, "main");

            // Create the parameter table
            lua_newtable(mLuaState);

            // Push the command line table
            lua_newtable(mLuaState);

            for (Kiaro::Common::S32 iteration = 0; iteration < argc; iteration++)
            {
                lua_pushinteger(mLuaState, iteration + 1);
                lua_pushstring(mLuaState, argv[iteration]);
                lua_settable(mLuaState, -3);
            }
            lua_call(mLuaState, 2, 0);

            Support::Console::write(Support::Console::MESSAGE_INFO, "SEngineInstance: Initialized Lua.");
            return 0;
        }

        Common::U32 SEngineInstance::initializeRenderer(void)
        {
            irr::video::E_DRIVER_TYPE videoDriver = irr::video::EDT_OPENGL;
            Support::SSettingsRegistry* settings = Support::SSettingsRegistry::getPointer();

            irr::SIrrlichtCreationParameters creationParameters;

            if (mEngineMode != Core::MODE_DEDICATED)
            {
                Common::S32 monitorCount = al_get_num_video_adapters();
                Support::Console::write(Support::Console::MESSAGE_INFO, "SEngineInstance: Detected %u monitor(s)", monitorCount);

                // Create the Allegro window and get its ID
                al_set_new_display_flags(ALLEGRO_GENERATE_EXPOSE_EVENTS | ALLEGRO_RESIZABLE);

                irr::core::dimension2d<Common::U32> resolution = settings->getValue<irr::core::dimension2d<Common::U32>>("Video::Resolution");
                mDisplay = al_create_display(resolution.Width, resolution.Height);

                // TODO (Robert MacGregor#9): Use a preference for the desired screen
                if (!mDisplay)
                {
                    Support::Console::write(Support::Console::MESSAGE_FATAL, "SEngineInstance.cpp: Failed to create Allegro display!");
                    return 1;
                }

                mWindowEventQueue = al_create_event_queue();
                al_register_event_source(mWindowEventQueue, al_get_display_event_source(mDisplay));

                al_set_window_title(mDisplay, "Kiaro Game Engine");

                #if defined(ENGINE_UNIX)
                    XID windowID = al_get_x_window_id(mDisplay);
                    creationParameters.WindowId = reinterpret_cast<void*>(windowID);
                #elif defined(ENGINE_WIN)
                    HWND windowHandle = al_get_win_window_handle(mDisplay);
                    creationParameters.WindowId = reinterpret_cast<void*>(windowHandle);
                #else
                    NSWindow* windowHandle = al_osx_get_window(mDisplay);
                    creationParameters.WindowId = reinterpret_cast<void*>(windowHandle);
                #endif
            }
            else
                videoDriver = irr::video::EDT_NULL;

            // Setup the Irrlicht creation request
            creationParameters.Bits = 32;
            creationParameters.IgnoreInput = false; // We will use Allegro for this
            creationParameters.DriverType = videoDriver;

            mIrrlichtDevice = irr::createDeviceEx(creationParameters);

            // Grab the scene manager and store it to reduce a function call
            mSceneManager = mIrrlichtDevice->getSceneManager();
            mSceneManager->setActiveCamera(mSceneManager->addCameraSceneNode());

            // Initialize the main scene and set it
            // TODO (Robert MacGregor#9): Only initialize when running as a client.
            mMainScene = new Video::CSceneGraph();
            this->setSceneGraph(mMainScene);

            Support::Console::write(Support::Console::MESSAGE_INFO, "SEngineInstance: Irrlicht version is %s.", mIrrlichtDevice->getVersion());
            Support::Console::write(Support::Console::MESSAGE_INFO, "SEngineInstance: Initialized renderer.");

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
            // What is our current display size?
            irr::core::dimension2d<irr::u32> lastDisplaySize = mIrrlichtDevice->getVideoDriver()->getScreenSize();

            // Start the Loop
            Common::F32 deltaTimeSeconds = 0.0f;

            while (mRunning && mIrrlichtDevice->run())
            {
                try
                {
                    // Update all our subsystems
                    Support::FTime::timer timerID = Support::FTime::startTimer();
                    Core::Tasking::SAsynchronousTaskManager::getPointer()->tick();

                    // Pump a time pulse at the scheduler
                    Support::SSynchronousScheduler::getPointer()->update();

                    // The GUI, video and sound systems run independently of our network time pulse
                    if (mEngineMode == Core::MODE_CLIENT || mEngineMode == Core::MODE_CLIENTCONNECT)
                    {
                        irr::core::dimension2d<Common::U32> currentDisplaySize = mIrrlichtDevice->getVideoDriver()->getScreenSize();

                        // Be sure to notify all the subsystems of our window resizing
                        if (lastDisplaySize != currentDisplaySize)
                        {
                            CEGUI::Sizef newDisplaySize(currentDisplaySize.Width, currentDisplaySize.Height);
                            CEGUI::System::getSingleton().notifyDisplaySizeChanged(newDisplaySize);

                            lastDisplaySize = currentDisplaySize;
                        }

                        CEGUI::System::getSingleton().injectTimePulse(deltaTimeSeconds);

                        // Since we're a client, render the frame right after updating
                        mIrrlichtDevice->getVideoDriver()->beginScene(true, true);
                        mSceneManager->drawAll();

                        CEGUI::System::getSingleton().renderAllGUIContexts();

                        mIrrlichtDevice->getVideoDriver()->endScene();

                        if (mDisplay)
                            this->processWindowEvents();
                    }

                    deltaTimeSeconds = Support::FTime::stopTimer(timerID);
                }
                catch(std::exception& e)
                {
                    Support::Console::write(Support::Console::MESSAGE_ERROR, "SEngineInstance: An internal exception of type '%s' has occurred:\n%s", typeid(e).name(), e.what());

                    // Something is probably up, we should leave.
                //    Net::SClient::destroy();

                    // Servers just drop off the client that it last processed
                    Game::SGameServer* server = Game::SGameServer::getPointer();
                    if (server)
                    {
                        Net::CIncomingClient* lastClient = server->getLastPacketSender();

                        // TODO (Robert MacGregor#9): Handle for if an exception is thrown AND we are running as a listen server.
                        if (lastClient)
                            lastClient->disconnect("Internal Exception");
                    }
                }
            }
        }

        void SEngineInstance::processWindowEvents(void)
        {
            // Query for display events
            ALLEGRO_EVENT windowEvent;

            if (al_get_next_event(mWindowEventQueue, &windowEvent))
                switch (windowEvent.type)
                    {
                        case ALLEGRO_EVENT_DISPLAY_CLOSE:
                        {
                            this->kill();
                            break;
                        }

                        case ALLEGRO_EVENT_DISPLAY_RESIZE:
                        {
                            if (!al_acknowledge_resize(mDisplay))
                                Support::Console::write(Support::Console::MESSAGE_WARNING, "SEngineInstance: Failed to resize display!");
                            else
                            {
                                // What is the new display dimensions?
                                Common::S32 displayWidth = al_get_display_width(mDisplay);
                                Common::S32 displayHeight = al_get_display_height(mDisplay);

                                mIrrlichtDevice->getVideoDriver()->OnResize(irr::core::dimension2d<Common::U32>(displayWidth, displayHeight));
                            }

                            break;
                        }

                        case ALLEGRO_EVENT_DISPLAY_SWITCH_OUT:
                        {
                            break;
                        }

                        case ALLEGRO_EVENT_DISPLAY_SWITCH_IN:
                        {
                            break;
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
                syncScheduler->schedule<Input::SInputListener>(13, true, inputListener, &Input::SInputListener::update);
            }

            syncScheduler->schedule(ENGINE_TICKRATE, true, this, &SEngineInstance::networkUpdate);
        }

        void SEngineInstance::setSceneGraph(Video::CSceneGraph* graph)
        {
            if (mCurrentScene)
                mCurrentScene->setVisible(false);

            mCurrentScene = graph;
            mCurrentScene->setVisible(true);
        }
    } // End Namespace Engine
} // End Namespace Kiaro
