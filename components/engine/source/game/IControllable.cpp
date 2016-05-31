/**
 *  @file IControllable.cpp
 */

#include <game/IControllable.hpp>

namespace Kiaro
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
