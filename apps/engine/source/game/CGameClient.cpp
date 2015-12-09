/**
 */

#include <net/IServer.hpp>
#include <game/CGameClient.hpp>

namespace Kiaro
{
    namespace Game
    {
        CGameClient::CGameClient(Net::RemoteHostContext client) : Net::IIncomingClient(client, nullptr)
        {

        }
    }
}
