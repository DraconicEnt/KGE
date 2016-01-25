/**
 *  @file IGameMode.hpp
 *  @brief Include file defining the Server and related functions.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_GAME_IGAMEMODE_HPP_
#define _INCLUDE_GAME_IGAMEMODE_HPP_

#include <easydelegate/easydelegate.hpp>

#include "support/common.hpp"
#include <support/String.hpp>

namespace Kiaro
{
    namespace Game
    {
        class IGameMode
        {
            // Public Members
            public:
                const Support::String mName;
                
            // Public Methods
            public:
                IGameMode(const Support::String& name = "Unnamed") : mName(name) { }
                
                virtual void setup(void) { std::cerr << "IGameMode: Gamemode setup not implemented!" << std::endl; }
                virtual void tearDown(void) { std::cerr << "IGameMode: Gamemode teardown not implemented!" << std::endl; }

                virtual void onPlayerKilled(void) { }

        };
    } // End Namespace Game
} // End Namespace Kiaro
#endif // _INCLUDE_GAME_IGAMEMODE_HPP_
