/**
 *  @file CoreSingleton.hpp
 *  @brief The common include file for the Kiaro application defining things such as error values.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_ENGINE_CORESINGLETON_HPP_
#define _INCLUDE_KIARO_ENGINE_CORESINGLETON_HPP_

#include <allegro5/allegro.h>

#include <lua.hpp>
#include <irrlicht.h>

#include <support/support.hpp>
#include <support/common.hpp>

namespace Kiaro
{
    namespace Net
    {
        class IServer;
    }

    namespace Game
    {
        class SGameServer;
    }

    namespace Video
    {
        class CSceneGraph;
    }

    namespace Core
    {
        class COutgoingClient;

        enum MODE_NAME
        {
            MODE_CLIENT = 0,
            MODE_CLIENTCONNECT = 1,
            MODE_DEDICATED = 2,
        }; // End Enum ENGINE_MODE

        /**
         *  @brief The SEngineInstance class is the core of the entire game
         *  engine architecture.
         */
        class SEngineInstance
        {
            // Public Methods
            public:
                /**
                 *  @brief Returns an instance to the engine instance
                 *  singleton, allocating it if necessary.
                 *  @return A pointer to the currently allocated SEngineInstance. If
                 *  this is the first call, then a new SEngineInstance will be allocated.
                 */
                static SEngineInstance* getPointer(void);

                /**
                 *  @brief Destroys the engine instance singleton, performing any necessary
                 *  cleanup procedures before exiting gracefully.
                 */
                static void destroy(void);

                /**
                 *  @brief Sets the running mode of the game engine.
                 *  @param mode The mode enumeration value to use when starting
                 *  the engine.
                 *  @warning This should only be used before the Kiaro::Engine::SEngineInstance::start
                 *  call.
                 */
                void setMode(const MODE_NAME& mode);


                /**
                 *  @brief Sets the targeted server for startup connection sequences. If the connection
                 *  fails, then the engine will fall back to the normal startup procedures.
                 *  @param address The IP address to connect to when ready.
                 *  @param port The port number to connect on.
                 *  @warning This should only be used before the Kiaro::Engine::SEngineInstance::start
                 *  call.
                 */
                void setTargetServer(Common::C8* address, Common::U16 port);

                /**
                 *  @brief Notifies the engine of what game to run.
                 *  @param gameName The name of the game to run. This will be the name of a folder that is
                 *  in the same directory as the engine executable.
                 *  @warning This should only be used before the Kiaro::Engine::SEngineInstance::start
                 *  call.
                 */
                void setGame(const Support::String& gameName);

                irr::IrrlichtDevice* getIrrlichtDevice(void);

                irr::scene::ISceneManager* getSceneManager(void);

                Common::S32 start(const Common::S32& argc, Common::C8* argv[]);
                void kill(void);

                void setSceneGraph(Video::CSceneGraph* graph);

                /**
                 *  @brief Returns whether or not the engine is running as a dedicated server at the
                 *  time of this call.
                 *  @return A boolean representing whether or not the engine is currently running as
                 *  a dedicated server.
                 */
                bool isDedicated(void);

                lua_State* getLuaState(void);

            // Public Members
            public:
             //   Common::ColorRGBA mClearColor;

            // Private Methods
            private:
                //! Privately declared standard constructor to enforce singleton behavior.
                SEngineInstance(void);
                //! Privately declared standard destructor to enforce singleton behavior.
                ~SEngineInstance(void);

                void networkUpdate(void);

                int initializeGUI(void);

                /**
                 *  @brief A helper method to initialize the Lua scripting engine.
                 *  @param argc The number of arguments contained in argv. This value
                 *  should be passed in unmolested all the way from main.
                 *  @param argv A space delineated array of command line parameters
                 *  passed to the application. This value should be passed in unmolested all
                 *  the way from main.
                 */
                Common::U32 initializeLua(const Common::S32& argc, Common::C8* argv[]);

                //! A subroutine that is called to initialize the renderer.
                void initializeRenderer(void);

                //! A subroutine that is called to initialize the netcode.
                Common::U32 initializeNetwork(void);

                /**
                 *  @brief A helper method that is called to initialize the sound code.
                 */
                Common::U32 initializeSound(void);

                void initializeFileSystem(const Common::S32& argc, Common::C8* argv[]);

                void runGameLoop(void);

            // Private Members
            private:
                //! A boolean representing whether or not the engine is running
                bool mRunning;
                //! An enumeration representing the engine run status.
                MODE_NAME mEngineMode;

                //! A pointer to the Irrlicht renderer device.
                irr::IrrlichtDevice* mIrrlichtDevice;
                //! A pointer to the Irrlicht scene manager.
                irr::scene::ISceneManager* mSceneManager;
                /**
                 *  @brief A pointer to the main scene graph that the engine will
                 *  use to render the main game simulation.
                 */
                Video::CSceneGraph* mMainScene;

                Video::CSceneGraph* mCurrentScene;

                Common::C8* mTargetServerAddress;
                Common::U16 mTargetServerPort;


                //! A pointer to the Lua scripting engine instance.
                lua_State* mLuaState;

                //! The name of the currently running game name.
                Support::String mGameName;

                COutgoingClient* mActiveClient;

                ALLEGRO_DISPLAY* mDisplay;

                //typedef EasyDelegate::CachedDelegate<void> EngineTimePulseDelegate;
        };
    } // End Namespace Engine
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_ENGINE_CORESINGLETON_HPP_
