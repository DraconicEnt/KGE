/**
 *  @file CGameClient.cpp
 */

#include <net/IServer.hpp>
#include <game/CGameClient.hpp>
#include <game/IControllable.hpp>

namespace Kiaro
{
    namespace Game
    {
        CGameClient::CGameClient(Net::RemoteHostContext client) : Net::IIncomingClient(client, nullptr), mControlObject(nullptr)
        {

        }
        
        void CGameClient::setControlObject(IControllable* object)
        {
            mControlObject = object;
            
            if (object)
                object->mController = this;
        }
        
        IControllable* CGameClient::getControlObject(void) const
        {
            return mControlObject;
        }
    }
}
