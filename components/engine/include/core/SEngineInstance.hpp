/**
 *  @file SEngineInstance.hpp
 *  @brief Include file declaring the SEngineInstance class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_CORE_SENGINEINSTANCE_HPP_
#define _INCLUDE_CORE_SENGINEINSTANCE_HPP_

#include <allegro5/allegro.h>

#include <irrlicht.h>

#include <support/support.hpp>
#include <support/common.hpp>
#include <support/SSynchronousScheduler.hpp>

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

        /**
         *  @brief The SEngineInstance class is the core of the entire game
         *  engine architecture.
         */
        class SEngineInstance
        {
            // Public Members
            public:
                //! An enumeration used to represent the possible modes of operation for the engine.
                enum MODE_NAME
                {
                    //! Start up using the regular start process. Cinamatics, main menu, etc.
                    MODE_CLIENT = 0,
                    //! Start up and immediately connect to some remote server as soon as possible.
                    MODE_CLIENTCONNECT = 1,
                    //! Run as a console-only server; pretty helpful for CLI only boxes.
                    MODE_DEDICATED = 2,
                }; // End Enum ENGINE_MODE

            // Public Methods
            public:
                /**
                 *  @brief Returns an instance to the engine instance singleton, allocating it
                 *  if necessary.
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

                /**
                 *  @brief Tells the game engine to actually start and take control
                 *  flow away from the calling code.
                 *  @param argc The number of arguments contained in argv. This value
                 *  should be passed in unmolested all the way from main.
                 *  @param argv A space delineated array of command line parameters
                 *  passed to the application. This value should be passed in unmolested all
                 *  the way from main.
                 *  @return The return code of the engine process.
                 *  @retval 0 No error.
                 *  @retval !=0 A fatal error has occurred during the engine's lifetime that
                 *  caused it to exit prematurely.
                 */
                Common::S32 start(const Common::S32 argc, Common::C8* argv[]);

                /**
                 *  @brief Notifies the engine to stop at the next chance it gets.
                 */
                void kill(void);

                /**
                 *  @brief Returns whether or not the engine is running as a dedicated server at the
                 *  time of this call.
                 *  @return A boolean representing whether or not the engine is currently running as
                 *  a dedicated server.
                 */
                bool isDedicated(void);

				/**
				 *	@brief Toggles the performance statistic reporting to stdout. Even if this is off, the
				 *	data collection is still occurring.
				 *	@param enabled Whether or not the performance statistic reporting should occur.
				 */
				void setPerfStatEnabled(const bool enabled);

            // Private Methods
            private:
                //! Privately declared standard constructor to enforce singleton behavior.
                SEngineInstance(void);
                //! Privately declared standard destructor to enforce singleton behavior.
                ~SEngineInstance(void);

                void networkUpdate(void);

                /**
                 *  @brief A helper method used to provide the actual main loop of the
                 *  game engine.
                 */
                void runGameLoop(void);

				/**
				 *	@brief Internal method called by the synchronous scheduler once every ~4sec if perfstat
				 *	is enabled.
				 */
				void printPerfStat(void);

                /**
                 *  @brief A subroutine to initialize the GUI system.
                 *  @return The status code of the GUI initialization.
                 *  @retval 0 No error.
                 *  @retval !=0 An error has occurred in the GUI initialization.
                 */
                int initializeGUI(void);

                /**
                 *  @brief A subroutine to initialize the Lua scripting engine.
                 *  @param argc The number of arguments contained in argv. This value
                 *  should be passed in unmolested all the way from main.
                 *  @param argv A space delineated array of command line parameters
                 *  passed to the application. This value should be passed in unmolested all
                 *  the way from main.
                 *  @return The status code of the Lua runtime initialization.
                 *  @retval 0 No error.
                 *  @retval !=0 An error has occurred in the Lua runtime initialization.
                 */
                Common::U32 initializeLua(const Common::S32 argc, Common::C8* argv[]);

                /**
                 *  @brief A subroutine that is called to initialize the renderer.
                 *  @return The status code of the renderer initialization.
                 *  @retval 0 No error.
                 *  @retval !=0 An error has occurred in renderer initialization.
                 */
                Common::U32 initializeRenderer(void);

                /**
                 *  @brief A subroutine that is called to initialize the netcode.
                 *  @return The status code of the netcode initialization.
                 *  @retval 0 No error.
                 *  @retval !=0 An error has occurred in netcode initialization.
                 */
                Common::U32 initializeNetwork(void);

                /**
                 *  @brief A subroutine that is called to initialize the sound code.
                 *  @return The status code of the sound system initialization.
                 *  @retval 0 No error.
                 *  @retval !=0 An error has occurred in the sound system initialization.
                 */
                Common::U32 initializeSound(void);

                /**
                 *  @brief A subroutine that is called to initialize the file system.
                 *  @param argc The number of arguments contained in argv. This value
                 *  should be passed in unmolested all the way from main.
                 *  @param argv A space delineated array of command line parameters
                 *  passed to the application. This value should be passed in unmolested all
                 *  the way from main.
                 *  @return The status code of the file system initialization.
                 *  @retval 0 No error.
                 *  @retval !=0 An error has occurred in the file system initialization.
                 */
                void initializeFileSystem(const Common::S32 argc, Common::C8* argv[]);

                void initializeScheduledEvents(void);

            // Private Members
            private:
                //! A boolean representing whether or not the engine is running
                bool mRunning;
                //! An enumeration representing the engine run status.
                MODE_NAME mEngineMode;

                Common::C8* mTargetServerAddress;
                Common::U16 mTargetServerPort;

                //! The name of the currently running game name.
                Support::String mGameName;

                //! A pointer to the active outgoing client.
                COutgoingClient* mActiveClient;

				Support::CScheduledEvent* mPerfStatSchedule;
        };
    } // End Namespace Engine
} // End Namespace Kiaro
#endif // _INCLUDE_CORE_SENGINEINSTANCE_HPP_
