/**
 */

#ifndef _INCLUDE_GAME_SGAMESERVER_HPP_
#define _INCLUDE_GAME_SGAMESERVER_HPP_

#include <net/IServer.hpp>

namespace Kiaro
{
    namespace Game
    {
        class IGameMode;

        class SGameServer : public Net::IServer
        {
            public:
                static SGameServer* getPointer(void);
                static void destroy(void);

                void setGamemode(IGameMode* game);

                void update(const Common::F32& deltaTimeSeconds);

            private:
                SGameServer(const Support::String& listenAddress, const Common::U16& listenPort, const Common::U32& maximumClientCount);
                ~SGameServer(void);

                Game::IGameMode* mCurrentGamemode;
        };
    } // End NameSpace Game
} // End NameSpace Kiaro
#endif // _INCLUDE_GAME_SGAMESERVER_HPP_
