/**
 *  @file CGameAIClient.cpp
 *  @brief Source file implementing the CGameAIClient class methods.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <game/CGameAIClient.hpp>

namespace Kiaro
{
    namespace Engine
    {
        namespace Game
        {
            CGameAIClient::CGameAIClient(void) : CGameClient(nullptr)
            {
            }

            void CGameAIClient::disconnect(const Support::String& reason)
            {
                // TODO (Robert MacGregor#9): Handle AI-disconnect call?
            }

            void CGameAIClient::send(Net::IMessage* packet, const bool reliable)
            {
                // TODO (Robert MacGregor#9): Handle AI-packet sends?
            }
        } // End NameSpace Game
    }
} // End Namespace Kiaro
