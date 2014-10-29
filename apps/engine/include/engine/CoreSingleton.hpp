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

#include <irrlicht/irrlicht.h>

#include <engine/Common.hpp>

namespace Kiaro
{
    enum ENGINE_MODE
    {
        ENGINE_CLIENT = 0,
        ENGINE_CLIENTCONNECT = 1,
        ENGINE_DEDICATED = 2,
    }; // End Enum ENGINE_MODE

    namespace Game
    {
        class OutgoingClient;
        class ServerSingleton;
    }

    namespace Network
    {
        class ServerBase;
        class OutgoingClientBase;
    }

    namespace Engine
    {
        class CoreSingleton
        {
            // Public Methods
            public:
                static CoreSingleton *getPointer(void);
                static void destroy(void);

                void setMode(const ENGINE_MODE mode);
                void setTargetServer(Kiaro::Common::C8 *address, Kiaro::Common::U16 port);
                void setGame(const std::string &gameName);

                irr::IrrlichtDevice *getIrrlichtDevice(void);

                Kiaro::Common::U32 run(Kiaro::Common::S32 argc, Kiaro::Common::C8 *argv[]);
                void kill(void);

            // Public Members
            Kiaro::Common::ColorRGBA mClearColor;

            // Private Methods
            private:
                //! Standard constructor
                CoreSingleton(void);
                //! Standard destructor
                ~CoreSingleton(void);

            // Private Members
            private:
                //! A boolean representing whether or not the engine is running
                bool mRunning;
                //! An enumeration representing the engine
                ENGINE_MODE mEngineMode;
                irr::IrrlichtDevice *mIrrlichtDevice;

                Kiaro::Common::C8 *mTargetServerAddress;
                Kiaro::Common::U16 mTargetServerPort;
                Kiaro::Network::OutgoingClientBase *mClient;
                Kiaro::Network::ServerBase *mServer;

                std::string mGameName;
        };
    } // End Namespace Engine
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_ENGINE_CORESINGLETON_HPP_
