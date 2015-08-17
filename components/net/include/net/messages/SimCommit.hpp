/**
 *  @file SimCommit.hpp
 *  @brief The common include file for the Kiaro application defining things such as error values.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_GAME_PACKETS_SIMCOMMIT_HPP_
#define _INCLUDE_KIARO_GAME_PACKETS_SIMCOMMIT_HPP_

#include <stdexcept>

#include <net/messages/types.hpp>
#include <net/messages/IMessage.hpp>

namespace Kiaro
{
    namespace Net
    {
        class CIncomingClient;
        class NetworkedEntity;

        namespace Messages
        {
            class SimCommit : public IMessage
            {
                // Public Methods
                public:
                    SimCommit(Support::CBitStream* in = NULL, CIncomingClient* sender = NULL);

                    virtual void packEverything(Support::CBitStream& out) const;
                    virtual void unpack(Support::CBitStream& in);
                    Common::U32 getMinimumPacketPayloadLength(void);
                    size_t getRequiredMemory(void);
            };
        } // End NameSpace Packets
    } // End NameSpace Game
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_GAME_PACKETS_SIMCOMMIT_HPP_
