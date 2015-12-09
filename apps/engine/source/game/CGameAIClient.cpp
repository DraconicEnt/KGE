/**
 */

#include <game/CGameAIClient.hpp>

namespace Kiaro
{
    namespace Game
    {
        CGameAIClient::CGameAIClient(void) : Net::IIncomingClient(NULL, NULL)
        {

        }

        void CGameAIClient::disconnect(const Support::String& reason)
        {
            // TODO (Robert MacGregor#9): Handle AI-disconnect call?
        }

        void CGameAIClient::send(Net::Messages::IMessage* packet, const bool& reliable)
        {
            // TODO (Robert MacGregor#9): Handle AI-packet sends?
        }
    } // End NameSpace Game
} // End Namespace Kiaro
