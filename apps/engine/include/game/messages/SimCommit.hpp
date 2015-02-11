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

#include <network/MessageBase.hpp>

namespace Kiaro
{
    namespace Network
    {
        class IncomingClient;
        class NetworkedEntity;
    } // End NameSpace Network

    namespace Game
    {
        namespace Messages
        {
            class SimCommit : public Kiaro::Network::MessageBase
            {
                // Public Methods
                public:
                    SimCommit(Kiaro::Support::BitStream *in = NULL, Kiaro::Network::IncomingClientBase *sender = NULL) : Kiaro::Network::MessageBase(MESSAGE_SIMCOMMIT, in, sender)
                    {

                    }

                    void packData(Kiaro::Support::BitStream &out)
                    {
                        Kiaro::Network::MessageBase::packData(out);
                    }

                    void unpackData(Kiaro::Support::BitStream &in)
                    {
                        if (in.getSize() <= getMinimumPacketPayloadLength())
                            throw std::runtime_error("Unable to unpack SimCommit packet; too small of a payload!");
                    }

                    Kiaro::Common::U32 getMinimumPacketPayloadLength(void)
                    {
                        return Kiaro::Network::MessageBase::getMinimumPacketPayloadLength();
                    }
            };
        } // End NameSpace Packets
    } // End NameSpace Game
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_GAME_PACKETS_SIMCOMMIT_HPP_
