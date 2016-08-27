/**
 *  @file IControllable.cpp
 *  @brief Include file declaring the IControllable interface class and its methods..
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_GAME_ICONTROLLABLE_HPP_
#define _INCLUDE_GAME_ICONTROLLABLE_HPP_

#include <game/CMove.hpp>

namespace Kiaro
{
    namespace Game
    {
        class CGameClient;

        /**
         *  @brief IControllable is an interface class for objects that may be implemented by
         *  objects that can be controlled by both Human and computer generated input sources.
         */
        class IControllable
        {
            friend class CGameClient;

            // Protected Members
            protected:
                //! Pointer to the controlling client.
                CGameClient* mController;

            // Public Methods
            public:
                //! Parameter-less constructor.
                IControllable(void);

                /**
                 *  @brief Returns the current
                 */
                CGameClient* getController(void) const NOTHROW;
        };
    }
}
#endif // _INCLUDE_GAME_ICONTROLLABLE_HPP_
