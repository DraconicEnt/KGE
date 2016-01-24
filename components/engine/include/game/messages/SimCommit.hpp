/**
 *  @file SimCommit.hpp
 *  @brief The common include file for the Kiaro application defining things such as error values.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2015 Draconic Entity
 */

#ifndef _INCLUDE_KIARO_GAME_PACKETS_SIMCOMMIT_HPP_
#define _INCLUDE_KIARO_GAME_PACKETS_SIMCOMMIT_HPP_

#include <stdexcept>

#include <game/messages/types.hpp>
#include <net/IMessage.hpp>

namespace Kiaro
{
    namespace Net
    {
        class IIncomingClient;
        class NetworkedEntity;
    }
    
    namespace Game
    {
        namespace Messages
        {
            class SimCommit : public Net::IMessage
            {
                // Public Methods
                public:
                    SimCommit(Support::CBitStream* in = NULL, Net::IIncomingClient* sender = NULL);

                    virtual void packEverything(Support::CBitStream& out) const;
                    virtual void unpack(Support::CBitStream& in);
                    Common::U32 getMinimumPacketPayloadLength(void);
                    size_t getRequiredMemory(void);
            };
        } // End NameSpace Packets
    } // End NameSpace Game
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_GAME_PACKETS_SIMCOMMIT_HPP_
