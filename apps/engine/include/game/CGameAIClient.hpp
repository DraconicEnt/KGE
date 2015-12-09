/**
 */

#ifndef _INCLUDE_GAME_CGAMEAICLIENT_HPP_
#define _INCLUDE_GAME_CGAMEAICLIENT_HPP_

#include <support/String.hpp>

#include <net/IIncomingClient.hpp>

namespace Kiaro
{
    namespace Game
    {
        class CGameAIClient : public Net::IIncomingClient
        {
            public:
                CGameAIClient(void);

                void disconnect(const Support::String& reason);
                void send(Net::Messages::IMessage* packet, const bool& reliable);
        };
    } // End NameSpace Game
} // End Namespace Kiaro
#endif // _INCLUDE_GAME_CAICLIENT_HPP_
