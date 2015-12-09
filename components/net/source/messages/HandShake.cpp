/**
 *  @file HandShake.cpp
 *  @brief The common include file for the Kiaro application defining things such as error values.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2015 Draconic Entity
 */

#include <stdexcept>

#include <net/messages/HandShake.hpp>
#include <net/IIncomingClient.hpp>

namespace Kiaro
{
    namespace Net
    {
        namespace Messages
        {
            HandShake::HandShake(Support::CBitStream* in, IIncomingClient* sender) : IMessage(TYPE_HANDSHAKE, in, sender),
            mVersionMajor(VERSION::MAJOR), mVersionMinor(VERSION::MINOR), mVersionRevision(VERSION::REVISION),
            mVersionBuild(VERSION::BUILD), mProtocolVersion(VERSION::PROTOCOL)
            {

            }

            void HandShake::packEverything(Support::CBitStream& out) const
            {
                out << mVersionMajor << mVersionMinor << mVersionRevision << mVersionBuild << mProtocolVersion;

                IMessage::packEverything(out);
            }

            void HandShake::unpack(Support::CBitStream& in)
            {
                if (in.getPointer() < this->getMinimumPacketPayloadLength())
                    throw std::underflow_error("Unable to unpack HandShake packet; too small of a payload!");

                in >> mVersionMajor >> mVersionMinor >> mVersionRevision >> mVersionBuild >> mProtocolVersion;
            }

            Common::U32 HandShake::getMinimumPacketPayloadLength(void)
            {
                return (sizeof(Common::U32) * 2) + (sizeof(Common::U8) * 3);
            }

            size_t HandShake::getRequiredMemory(void)
            {
                return (sizeof(Common::U8) * 3) + (sizeof(Common::U32) * 2) + IMessage::getRequiredMemory();
            }
        } // End NameSpace Messages
    } // End NameSpace Game
} // End NameSpace Kiaro

