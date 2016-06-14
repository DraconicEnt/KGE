/**
 *  @file SimCommit.cpp
 *  @brief The common include file for the Kiaro application defining things such as error values.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <stdexcept>

#include <net/IMessage.hpp>
#include <game/messages/SimCommit.hpp>

namespace Kiaro
{
    namespace Game
    {
        namespace Messages
        {
            SimCommit::SimCommit(Support::CBitStream* in, Net::IIncomingClient* sender) : IMessage(in, sender)
            {
            }

            void SimCommit::packEverything(Support::CBitStream& out) const
            {
                IMessage::packBaseData<SimCommit>(out);
            }

            void SimCommit::unpack(Support::CBitStream& in)
            {
                // FIXME (Robert MacGregor#9): Sim Commit Messages don't contain any values
                //if (in.getSize() <= getMinimumPacketPayloadLength())
                //    throw std::runtime_error("Unable to unpack SimCommit packet; too small of a payload!");
            }

            size_t SimCommit::getMinimumPacketPayloadLength(void) const
            {
                return IMessage::getMinimumPacketPayloadLength();
            }

            size_t SimCommit::getRequiredMemory(void) const
            {
                return IMessage::getRequiredMemory();
            }
        } // End NameSpace Packets
    } // End NameSpace Game
} // End NameSpace Kiaro
