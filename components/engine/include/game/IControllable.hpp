/**
 *  @file IControllable.hpp
 */

#ifndef _INCLUDE_GAME_ICONTROLLABLE_HPP_
#define _INCLUDE_GAME_ICONTROLLABLE_HPP_
#include <game/CMove.hpp>

namespace Kiaro
{
    namespace Game
    {
        class CGameClient;
        
        class IControllable
        {
            friend class CGameClient;
            
            // Protected Members
            protected:
                CGameClient* mController;
                
            // Public Methods
            public:
                IControllable(void);
                
                virtual void updateMove(const CMove& move) = 0;
                
                CGameClient* getController(void) const NOTHROW;
        };
    }
}
#endif // _INCLUDE_GAME_ICONTROLLABLE_HPP_
