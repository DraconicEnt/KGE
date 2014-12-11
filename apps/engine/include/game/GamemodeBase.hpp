/**
 *  @file ServerBase.hpp
 *  @brief Include file defining the Server and related functions.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_GAMEMODEBASE_HPP_
#define _INCLUDE_KIARO_GAMEMODEBASE_HPP_

#include "engine/Common.hpp"

#include <easydelegate.hpp>

namespace Kiaro
{
    namespace Game
    {
        class GamemodeBase
        {
            // Public Methods
            public:
                virtual void setup(void) { std::cerr << "GamemodeBase: Gamemode setup not implemented!" << std::endl; }
                virtual void tearDown(void) { std::cerr << "GamemodeBase: Gamemode teardown not implemented!" << std::endl; }

                virtual void OnPlayerKilled(void) { }
        };
    } // End Namespace Game
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_GAMEMODEBASE_HPP_
