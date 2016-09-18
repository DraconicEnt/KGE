/**
 *  @file CGameClient.cpp
 *  @brief Source file implementing the CGameClient class methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <net/IServer.hpp>
#include <game/CGameClient.hpp>
#include <game/IControllable.hpp>

#include <game/messages/messages.hpp>

namespace Kiaro
{
    namespace Engine
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

            void CGameClient::disconnect(const Support::String& reason)
            {
                Game::Messages::Disconnect disconnect;
                disconnect.mReason = reason;
                this->send(&disconnect, true);
                Net::IIncomingClient::disconnect(reason);
            }
        }
    }
}
