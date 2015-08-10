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


#include <allegro5/allegro.h>

#include <irrlicht.h>
#include <lua.hpp>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Irrlicht/Renderer.h>

#include <core/config.hpp>
#include <core/tasking/SAsynchronousTaskManager.hpp>
#include <core/tasking/SAsynchronousSchedulerTask.hpp>
#include <core/SEngineInstance.hpp>
#include <filesystem/SResourceProvider.hpp>
#include <input/SInputListener.hpp>
#include <core/SSettingsRegistry.hpp>

#include <game/SGameServer.hpp>

#include <net/SClient.hpp>

#include <net/CClient.hpp>

#include <support/FTime.hpp>
#include <core/SSynchronousScheduler.hpp>

#include <video/CSceneGraph.hpp>

#include <core/Logging.hpp>

namespace Kiaro
{
    namespace Core
    {
        static Kiaro::Core::SEngineInstance* sInstance = NULL;

        SEngineInstance *SEngineInstance::getPointer(void)
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

            this->initializeFileSystem(argc, argv);

            Core::Logging::write(Core::Logging::MESSAGE_INFO, "SEngineInstance: Running game '%s'", mGameName.data());

            // Add the game search path
            if (PHYSFS_mount(mGameName.c_str(), NULL, 1) == 0)
            {
                mRunning = false;

                Core::Logging::write(Core::Logging::MESSAGE_FATAL, "SEngineInstance: Failed to mount game directory '%s'. Reason: '%s'", mGameName.data(), PHYSFS_getLastError());
                return -1;
            }
            else
                Core::Logging::write(Core::Logging::MESSAGE_INFO, "SEngineInstance: Mounted game directory '%s' successfully.", mGameName.data());

            Core::SSettingsRegistry* settings = Core::SSettingsRegistry::getPointer();

            // TODO (Robert MacGregor#9): Return error codes for Lua, renderer, GUI and the netcode
            mRunning = this->initializeLua(argc, argv) == 0 ? true : false;
            if (!mRunning)
                return 1;

            // Init the taskers
            Core::Tasking::SAsynchronousTaskManager* asyncTaskManager = Core::Tasking::SAsynchronousTaskManager::getPointer();
            asyncTaskManager->addTask(Core::Tasking::SAsynchronousSchedulerTask::getPointer());

            this->initializeRenderer();

            mRunning = this->initializeGUI() == 0 ? true : false;
            if (!mRunning)
                return 1;

            this->initializeSound();
            this->initializeNetwork();

            this->runGameLoop();

            return 1;
        }

        void SEngineInstance::kill(void)
        {
            if (!mRunning)
                return;

            Core::Logging::write(Core::Logging::MESSAGE_INFO, "SEngineInstance: Killed via kill()");
            mRunning = false;
        }

        SEngineInstance::SEngineInstance(void) : mEngineMode(MODE_CLIENT), mIrrlichtDevice(NULL), mTargetServerAddress("127.0.0.1"), mTargetServerPort(11595),
        mRunning(false), mClearColor(Kiaro::Common::ColorRGBA(255, 255, 0, 0)), mLuaState(NULL), mCurrentScene(NULL)
        {

        }

        SEngineInstance::~SEngineInstance(void)
        {
            Core::Logging::write(Core::Logging::MESSAGE_INFO, "SEngineInstance: Deinitializing ...");

            // TODO: Check the destroy order
            Net::SClient::destroy();
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

            Core::SSettingsRegistry::destroy();

            PHYSFS_deinit();
            enet_deinitialize();
        }

        void SEngineInstance::networkUpdate(void)
        {
            // FIXME (Robert MacGregor#9): Pass in the time delta
            Net::SClient* client = Net::SClient::getPointer();
            if (client)
                client->networkUpdate();

            Game::SGameServer* server = Game::SGameServer::getPointer();
            if (server)
                server->networkUpdate(0.0f);
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
                    //EGUI::IrrlichtRenderer::bootstrapSystem(*mIrrlichtDevice);

                    resourceProvider->setResourceGroupDirectory("fonts", "fonts/");
                    resourceProvider->setResourceGroupDirectory("ui", "ui/");
                  //  resourceProvider->setResourceGroupDirectory("global", basePath);

                    CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme", "ui");
                    CEGUI::FontManager::getSingleton().createFromFile( "DejaVuSans-10.font", "fonts" );

                    // Set the defaults
                    CEGUI::GUIContext& guiContext = CEGUI::System::getSingleton().getDefaultGUIContext();

                    guiContext.setDefaultFont( "DejaVuSans-10" );
                    guiContext.getMouseCursor().setDefaultImage( "TaharezLook/MouseArrow" );
                    guiContext.getMouseCursor().setImage(guiContext.getMouseCursor().getDefaultImage());

                    Core::Logging::write(Core::Logging::MESSAGE_INFO, "SEngineInstance: Initialized the GUI system.");
                }
                catch (CEGUI::InvalidRequestException& e)
                {
                    Core::Logging::write(Core::Logging::MESSAGE_FATAL, "SEngineInstance: Failed to initialize the GUI System. Reason:\n%s", e.what());
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
                Core::Logging::write(Core::Logging::MESSAGE_FATAL, "SEngineInstance.cpp: Failed to load main.lua. Reason: '%s'", luaL_checkstring(mLuaState, -1));
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

            Core::Logging::write(Core::Logging::MESSAGE_INFO, "SEngineInstance: Initialized Lua.");
            return 0;
        }

        void SEngineInstance::initializeRenderer(void)
        {
            // Handle Execution Flag
            irr::video::E_DRIVER_TYPE videoDriver = irr::video::EDT_OPENGL;

            if (mEngineMode == Kiaro::Core::MODE_DEDICATED)
                videoDriver = irr::video::EDT_NULL;

            Core::SSettingsRegistry* settings = Core::SSettingsRegistry::getPointer();

            // Init the Input listener
            Input::SInputListener* inputListener = Input::SInputListener::getPointer();

            // Start up Irrlicht
            mIrrlichtDevice = irr::createDevice(videoDriver, settings->getValue<irr::core::dimension2d<Common::U32>>("Video::Resolution"), 32, false, false, false, inputListener);
            mIrrlichtDevice->setWindowCaption(L"Kiaro Game Engine");

            // Grab the scene manager and store it to reduce a function call
            mSceneManager = mIrrlichtDevice->getSceneManager();
            mSceneManager->addCameraSceneNode();

            // Initialize the main scene and set it
            // TODO (Robert MacGregor#9): Only initialize when running as a client.
            mMainScene = new Video::CSceneGraph();
            this->setSceneGraph(mMainScene);

            Core::Logging::write(Core::Logging::MESSAGE_INFO, "SEngineInstance: Irrlicht version is %s.", mIrrlichtDevice->getVersion());
            Core::Logging::write(Core::Logging::MESSAGE_INFO, "SEngineInstance: Initialized renderer.");
        }

        Common::U32 SEngineInstance::initializeNetwork(void)
        {
            // Catch if the netcode somehow doesn't initialize correctly.
            if (enet_initialize() < 0)
            {
                Core::Logging::write(Core::Logging::MESSAGE_FATAL, "SEngineInstance: Failed to initialize the network!");
                return 1;
            }

            // Initialize the client or server ends
            switch (mEngineMode)
            {
                case Core::MODE_CLIENT:
                {
                    Net::SClient::initialize(mIrrlichtDevice);
                    break;
                }

                case Core::MODE_CLIENTCONNECT:
                {
                    Net::SClient::initialize(mIrrlichtDevice);
                    Net::SClient* client = Net::SClient::getPointer();
                    client->connect(mTargetServerAddress, mTargetServerPort, 5000);

                    if (!client->getIsConnected())
                    {
                        Core::Logging::write(Core::Logging::MESSAGE_FATAL, "SEngineInstance: Failed to connect to remote host with srrver flag!");

                        Net::SClient::destroy();
                    }

                    break;
                }

                case Core::MODE_DEDICATED:
                {
                    Game::SGameServer::initialize();
                    break;
                }
            }

            Core::Logging::write(Core::Logging::MESSAGE_INFO, "SEngineInstance: Initialized network.");
            return 0;
        }

        void SEngineInstance::runGameLoop(void)
        {
            // What is our current display size?
            irr::core::dimension2d<irr::u32> lastDisplaySize = mIrrlichtDevice->getVideoDriver()->getScreenSize();

            // Start the Loop
            Common::F32 deltaTimeSeconds = 0.0f;

            // Setup and start the network time pulses
            Support::SSynchronousScheduler::getPointer()->schedule(ENGINE_TICKRATE, true, this, &SEngineInstance::networkUpdate);

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
                        mIrrlichtDevice->getVideoDriver()->beginScene(true, true, mClearColor);
                        mSceneManager->drawAll();

                        CEGUI::System::getSingleton().renderAllGUIContexts();

                        mIrrlichtDevice->getVideoDriver()->endScene();
                    }

                    deltaTimeSeconds = Support::FTime::stopTimer(timerID);
                }
                catch(std::exception& e)
                {
                    Core::Logging::write(Core::Logging::MESSAGE_ERROR, "SEngineInstance: An internal exception of type '%s' has occurred:\n%s", typeid(e).name(), e.what());

                    // Something is probably up, we should leave.
                    Net::SClient::destroy();

                    // Servers just drop off the client that it last processed
                    Game::SGameServer* server = Game::SGameServer::getPointer();
                    if (server)
                    {
                        Net::CClient* lastClient = server->getLastPacketSender();

                        // TODO (Robert MacGregor#9): Handle for if an exception is thrown AND we are running as a listen server.
                        if (lastClient)
                            lastClient->disconnect("Internal Exception");
                    }
                }
            }
        }

        Common::U32 SEngineInstance::initializeSound(void)
        {
            // Don't initialize the sound engine if we're running a dedicated server
            if (mEngineMode == Kiaro::Core::MODE_DEDICATED)
                return 0;

            #ifndef ENGINE_BUILD_SOUNDENGINE
                Core::Logging::write(Core::Logging::MESSAGE_WARNING, "SEngineInstance: Built without audio support. There will be no sound.");
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

        void SEngineInstance::setSceneGraph(Video::CSceneGraph* graph)
        {
            if (mCurrentScene)
                mCurrentScene->setVisible(false);

            mCurrentScene = graph;
            mCurrentScene->setVisible(true);
        }
    } // End Namespace Engine
} // End Namespace Kiaro
