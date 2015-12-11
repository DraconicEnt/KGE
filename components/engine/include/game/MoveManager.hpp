/**
 */

#ifndef _INCLUDE_GAME_MOVEMANAGER_HPP_
#define _INCLUDE_GAME_MOVEMANAGER_HPP_

#include <support/common.hpp>

namespace Kiaro
{
    namespace Game
    {
        class MoveManager
        {
            public:
                static Common::F32 moveForward;
                static Common::F32 moveBackward;
                static Common::F32 moveLeftward;
                static Common::F32 moveRightward;

                static void reset(void);
        };
    } // End NameSpace Game
} // End NameSpace Kiaro
#endif // _INCLUDE_GAME_SMOVEMANAGER_HPP_
