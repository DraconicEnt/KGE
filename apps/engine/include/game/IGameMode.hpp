/**
 *  @file IGameMode.hpp
 *  @brief Include file defining the Server and related functions.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_IGAMEMODE_HPP_
#define _INCLUDE_KIARO_IGAMEMODE_HPP_

#include <easydelegate.hpp>

#include "core/common.hpp"

namespace Kiaro
{
    namespace Game
    {
        class IGameMode
        {
            // Public Methods
            public:
                virtual void setup(void) { std::cerr << "IGameMode: Gamemode setup not implemented!" << std::endl; }
                virtual void tearDown(void) { std::cerr << "IGameMode: Gamemode teardown not implemented!" << std::endl; }

                virtual void onPlayerKilled(void) { }

        };
    } // End Namespace Game
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_IGAMEMODE_HPP_
