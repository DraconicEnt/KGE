/**
 *  @file ServerBase.hpp
 *  @brief Include file defining the Server and related functions.
 *
 *  This software is licensed under the GNU Lesser General Public License version 3.
 *  Please refer to gpl.txt and lgpl.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @version 0.1.0
 *  @date 12/23/2013
 *  @copyright (c) 2013 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_GAMEMODEBASE_HPP_
#define _INCLUDE_KIARO_GAMEMODEBASE_HPP_

#include "EngineCommon.hpp"

#include <easydelegate.hpp>

namespace Kiaro
{
    namespace Game
    {
        class GamemodeBase
        {
            // Public Methods
            public:
                virtual void Setup(void) { std::cerr << "GamemodeBase: Gamemode setup not implemented!" << std::endl; }
                virtual void Teardown(void) { std::cerr << "GamemodeBase: Gamemode teardown not implemented!" << std::endl; }

                virtual void OnPlayerKilled(void) { }
        };
    } // End Namespace Game
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_GAMEMODEBASE_HPP_
