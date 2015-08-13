/**
 */

#ifndef _INCLUDE_GAME_CGAMEAICLIENT_HPP_
#define _INCLUDE_GAME_CGAMEAICLIENT_HPP_

#include <support/String.hpp>

#include <net/CIncomingClient.hpp>

namespace Kiaro
{
    namespace Game
    {
        class CGameAIClient : public Net::CIncomingClient
        {
            public:
                CGameAIClient(void);

                void disconnect(const Support::String& reason);
                void send(Net::IMessage* packet, const bool& reliable);
        };
    } // End NameSpace Game
} // End Namespace Kiaro
#endif // _INCLUDE_GAME_CAICLIENT_HPP_
