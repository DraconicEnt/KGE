/**
 */

#ifndef _INCLUDE_GAME_CGAMEAICLIENT_HPP_
#define _INCLUDE_GAME_CGAMEAICLIENT_HPP_

#include <support/String.hpp>

#include <game/CGameClient.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Game
        {
            class CGameAIClient : public CGameClient
            {
                public:
                    CGameAIClient(void);

                    void disconnect(const Support::String& reason);
                    void send(Net::IMessage* packet, const bool reliable);
            };
        } // End NameSpace Game
    }
} // End Namespace Kiaro
#endif // _INCLUDE_GAME_CAICLIENT_HPP_
