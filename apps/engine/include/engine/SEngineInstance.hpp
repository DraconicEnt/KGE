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

#include <lua.hpp>
#include <irrlicht.h>
#include <easydelegate.hpp>

#include <engine/Common.hpp>

namespace Kiaro
{
    enum ENGINE_MODE
    {
        ENGINE_CLIENT = 0,
        ENGINE_CLIENTCONNECT = 1,
        ENGINE_DEDICATED = 2,
    }; // End Enum ENGINE_MODE

    namespace Network
    {
        class SServer;
        class SClient;
    }

    namespace Engine
    {
        class SEngineInstance
        {
            // Public Methods
            public:
                static SEngineInstance *getPointer(void);
                static void destroy(void);

                void setMode(const ENGINE_MODE mode);
                void setTargetServer(Kiaro::Common::C8 *address, Kiaro::Common::U16 port);
                void setGame(const Kiaro::Common::String &gameName);

                irr::IrrlichtDevice *getIrrlichtDevice(void);

                Kiaro::Common::U32 run(Kiaro::Common::S32 argc, Kiaro::Common::C8 *argv[]);
                void kill(void);

                bool isDedicated(void);

                lua_State *getLuaState(void);

            // Public Members
            public:
                Kiaro::Common::ColorRGBA mClearColor;

            // Private Methods
            private:
                //! Standard constructor
                SEngineInstance(void);
                //! Standard destructor
                ~SEngineInstance(void);

                void networkUpdate(void);
            // Private Members
            private:
                //! A boolean representing whether or not the engine is running
                bool mRunning;
                //! An enumeration representing the engine
                ENGINE_MODE mEngineMode;
                irr::IrrlichtDevice *mIrrlichtDevice;

                Kiaro::Common::C8 *mTargetServerAddress;
                Kiaro::Common::U16 mTargetServerPort;
                Kiaro::Network::SClient *mClient;
                Kiaro::Network::SServer *mServer;

                lua_State *mLuaState;

                Kiaro::Common::String mGameName;

                typedef EasyDelegate::CachedDelegate<void> EngineTimePulseDelegate;
        };
    } // End Namespace Engine
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_ENGINE_CORESINGLETON_HPP_
