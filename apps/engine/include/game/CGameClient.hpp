/**
 */

#ifndef _INCLUDE_GAME_CGAMECLIENT_HPP_
#define _INCLUDE_GAME_CGAMECLIENT_HPP_

#include <net/IIncomingClient.hpp>

namespace Kiaro
{
    namespace Game
    {
        class CGameClient : public Net::IIncomingClient
        {
            public:
                CGameClient(Net::RemoteHostContext client);
        };
    } // End NameSpace Game
} // End NameSpace Kiaro
#endif // _INCLUDE_GAME_CGAMECLIENT_HPP_
