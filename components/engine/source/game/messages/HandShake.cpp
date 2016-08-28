/**
 *  @file HandShake.cpp
 *  @brief The common include file for the Kiaro application defining things such as error values.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <stdexcept>

#include <game/messages/HandShake.hpp>
#include <net/IIncomingClient.hpp>

#include <support/support.hpp>

namespace Kiaro
{
    namespace Game
    {
        namespace Messages
        {
            HandShake::HandShake(Support::CBitStream* in, Net::IIncomingClient* sender) : IMessage(in, sender),
            mVersionMajor(VERSION::MAJOR), mVersionMinor(VERSION::MINOR), mVersionRevision(VERSION::REVISION), mVersionBuild(VERSION::BUILD),
            mProtocolVersion(VERSION::PROTOCOL)
            {
            }

            void HandShake::packEverything(Support::CBitStream& out) const
            {
                IMessage::packBaseData<HandShake>(out);

                out << mVersionMajor << mVersionMinor << mVersionRevision << mVersionBuild << mProtocolVersion;
            }

            void HandShake::unpack(Support::CBitStream& in)
            {
                if (in.getSize() - in.getPointer() < this->getMinimumPacketPayloadLength())
                    Support::throwException<std::underflow_error>("Unable to unpack HandShake packet; too small of a payload!");

                in >> mVersionMajor >> mVersionMinor >> mVersionRevision >> mVersionBuild >> mProtocolVersion;
            }

            size_t HandShake::getMinimumPacketPayloadLength(void) const
            {
                return (sizeof(Common::U32) * 2) + (sizeof(Common::U8) * 3);
            }

            size_t HandShake::getRequiredMemory(void) const
            {
                return (sizeof(Common::U8) * 3) + (sizeof(Common::U32) * 2) + IMessage::getRequiredMemory();
            }
        } // End NameSpace Messages
    } // End NameSpace Game
} // End NameSpace Kiaro

