/**
 *  @file CScriptedGame.cpp
 *  @brief INclude file declaring the CScriptedGame class and its methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_GAME_SCRIPTEDGAME_HPP_
#define _INCLUDE_GAME_SCRIPTEDGAME_HPP_

#include "game/IGameMode.hpp"

namespace Kiaro
{
    namespace Engine
    {
        namespace Game
        {
            class CScriptedGame : public IGameMode
            {
                // Public Methods
                public:
                    CScriptedGame(void);

                    void setup(void);
                    void tearDown(void);
            };
        } // End NameSpace Game
    }
} // End NameSpace Kiaro

#endif // _INCLUDE_GAME_SCRIPTEDGAME_HPP_
