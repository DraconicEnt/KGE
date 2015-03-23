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

#include <network/IMessage.hpp>

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
            class SimCommit : public Kiaro::Network::IMessage
            {
                // Public Methods
                public:
                    SimCommit(Kiaro::Support::CBitStream *in = NULL, Kiaro::Network::CClient *sender = NULL) : Kiaro::Network::IMessage(MESSAGE_SIMCOMMIT, in, sender)
                    {

                    }

                    void packData(Kiaro::Support::CBitStream &out)
                    {
                        Kiaro::Network::IMessage::packData(out);
                    }

                    void unpackData(Kiaro::Support::CBitStream &in)
                    {
                        // FIXME (Robert MacGregor#9): Sim Commit Messages don't contain any values
                        //if (in.getSize() <= getMinimumPacketPayloadLength())
                        //    throw std::runtime_error("Unable to unpack SimCommit packet; too small of a payload!");
                    }

                    Kiaro::Common::U32 getMinimumPacketPayloadLength(void)
                    {
                        return Kiaro::Network::IMessage::getMinimumPacketPayloadLength();
                    }
            };
        } // End NameSpace Packets
    } // End NameSpace Game
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_GAME_PACKETS_SIMCOMMIT_HPP_
