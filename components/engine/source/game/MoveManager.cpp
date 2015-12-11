/**
 */

#include <game/MoveManager.hpp>

namespace Kiaro
{
    namespace Game
    {
        Common::F32 MoveManager::moveForward = 0.0f;
        Common::F32 MoveManager::moveBackward = 0.0f;
        Common::F32 MoveManager::moveLeftward = 0.0f;
        Common::F32 MoveManager::moveRightward = 0.0f;

        void MoveManager::reset(void)
        {
            MoveManager::moveForward = 0.0f;
            MoveManager::moveBackward = 0.0f;
            MoveManager::moveLeftward = 0.0f;
            MoveManager::moveRightward = 0.0f;
        }
    } // End NameSpace Game
} // End NameSpace Kiaro
