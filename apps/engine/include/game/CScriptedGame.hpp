/**
 *  @file CScriptedGame.hpp
 */

#ifndef _INCLUDE_GAME_SCRIPTEDGAME_HPP_
#define _INCLUDE_GAME_SCRIPTEDGAME_HPP_

#include "game/IGameMode.hpp"

namespace Kiaro
{
    namespace Game
    {
        class CScriptedGame : public IGameMode
        {
            // Public Methods
            public:
                void setup(void);
                void tearDown(void);
        };
    } // End NameSpace Game
} // End NameSpace Kiaro

#endif // _INCLUDE_GAME_SCRIPTEDGAME_HPP_
