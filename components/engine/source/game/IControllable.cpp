/**
 *  @file IControllable.cpp
 *  @brief Source file implementing the IControllable interface class methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <game/IControllable.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Game
        {
            IControllable::IControllable(void) : mController(nullptr)
            {
            }

            CGameClient* IControllable::getController(void) const
            {
                return mController;
            }
        }
    }
}
