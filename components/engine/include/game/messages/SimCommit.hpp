/**
 *  @file SimCommit.hpp
 *  @brief The common include file for the Kiaro application defining things such as error values.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_KIARO_GAME_PACKETS_SIMCOMMIT_HPP_
#define _INCLUDE_KIARO_GAME_PACKETS_SIMCOMMIT_HPP_

#include <stdexcept>

#include <net/IMessage.hpp>

namespace Kiaro
{
    namespace Net
    {
        class IIncomingClient;
        class NetworkedEntity;
    }

    namespace Engine
    {
        namespace Game
        {
            namespace Messages
            {
                /**
                 *  @brief The SimCommit class is a server only message type that is used to signal to connected game clients that
                 *  the server is done submitting a simulation frame for the time being.
                 */
                class SimCommit : public Net::IMessage
                {
                    // Public Methods
                    public:
                        SimCommit(Support::CBitStream* in = nullptr, Net::IIncomingClient* sender = nullptr);

                        virtual void packEverything(Support::CBitStream& out) const;
                        virtual void unpack(Support::CBitStream& in);
                        size_t getMinimumPacketPayloadLength(void) const;
                        size_t getRequiredMemory(void) const;
                };
            } // End NameSpace Packets
        }
    } // End NameSpace Game
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_GAME_PACKETS_SIMCOMMIT_HPP_
