/**
 *  @file CScriptedGame.cpp
 *  @brief Source file implementing the CScriptedGame class methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */
#include <game/CScriptedGame.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Game
        {
            CScriptedGame::CScriptedGame(void) : IGameMode("Scripted Game")
            {
            }

            void CScriptedGame::setup(void)
            {
            }

            void CScriptedGame::tearDown(void)
            {
            }
        } // End NameSpace Game
    }
} // End NameSpace Kiaro
