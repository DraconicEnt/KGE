/**
 */

#ifndef _INCLUDE_GAME_CGAMECLIENT_HPP_
#define _INCLUDE_GAME_CGAMECLIENT_HPP_

#include <game/CMove.hpp>
#include <net/IIncomingClient.hpp>

namespace Kiaro
{
    namespace Game
    {        
        class IControllable;
        
        class CGameClient : public Net::IIncomingClient
        {
            // Public Members
            public:
                CMove mMove;
            
            // Protected Members
            protected:
                IControllable* mControlObject;
                
            // Public Methods
            public:
                CGameClient(Net::RemoteHostContext client);
                
                void setControlObject(IControllable* object);
                IControllable* getControlObject(void) const NOTHROW;
        };
    } // End NameSpace Game
} // End NameSpace Kiaro
#endif // _INCLUDE_GAME_CGAMECLIENT_HPP_
