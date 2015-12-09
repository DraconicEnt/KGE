/**
 *  @file SimCommit.cpp
 *  @brief The common include file for the Kiaro application defining things such as error values.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2015 Draconic Entity
 */

#include <stdexcept>

#include <net/messages/IMessage.hpp>
#include <net/messages/SimCommit.hpp>

namespace Kiaro
{
    namespace Net
    {
        namespace Messages
        {
            SimCommit::SimCommit(Support::CBitStream* in, IIncomingClient* sender) : IMessage(TYPE_SIMCOMMIT, in, sender)
            {

            }

            void SimCommit::packEverything(Support::CBitStream& out) const
            {
                IMessage::packEverything(out);
            }

            void SimCommit::unpack(Support::CBitStream& in)
            {
                // FIXME (Robert MacGregor#9): Sim Commit Messages don't contain any values
                //if (in.getSize() <= getMinimumPacketPayloadLength())
                //    throw std::runtime_error("Unable to unpack SimCommit packet; too small of a payload!");
            }

            Common::U32 SimCommit::getMinimumPacketPayloadLength(void)
            {
                return IMessage::getMinimumPacketPayloadLength();
            }

            size_t SimCommit::getRequiredMemory(void)
            {
                return IMessage::getRequiredMemory();
            }
        } // End NameSpace Packets
    } // End NameSpace Game
} // End NameSpace Kiaro
