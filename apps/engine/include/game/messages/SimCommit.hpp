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

#include <net/messages/messages.hpp>
#include <net/IMessage.hpp>

namespace Kiaro
{
    namespace Net
    {
        class IncomingClient;
        class NetworkedEntity;

        namespace Messages
        {
            class SimCommit : public Kiaro::Net::IMessage
            {
                // Public Methods
                public:
                    SimCommit(Kiaro::Support::CBitStream *in = NULL, Kiaro::Net::CClient *sender = NULL) : Kiaro::Net::IMessage(MESSAGE_SIMCOMMIT, in, sender)
                    {

                    }

                    void writeTo(Kiaro::Support::CBitStream &out) const
                    {
                        Kiaro::Net::IMessage::writeTo(out);
                    }

                    void readFrom(Kiaro::Support::CBitStream &in)
                    {
                        // FIXME (Robert MacGregor#9): Sim Commit Messages don't contain any values
                        //if (in.getSize() <= getMinimumPacketPayloadLength())
                        //    throw std::runtime_error("Unable to unpack SimCommit packet; too small of a payload!");
                    }

                    void extractFrom(Support::CBitStream &in)
                    {

                    }

                    Kiaro::Common::U32 getMinimumPacketPayloadLength(void)
                    {
                        return Kiaro::Net::IMessage::getMinimumPacketPayloadLength();
                    }

                    size_t getRequiredMemory(void)
                    {
                        return Net::IMessage::getRequiredMemory();
                    }
            };
        } // End NameSpace Packets
    } // End NameSpace Game
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_GAME_PACKETS_SIMCOMMIT_HPP_
