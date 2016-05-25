/**
 *  @file IMessage.cpp
 *  @brief Include file defining the Packet class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2015 Draconic Entity
 */

#include <net/IMessage.hpp>

namespace Kiaro
{
    namespace Net
    {
        IMessage::IMessage(const Common::U32 packetType, Support::CBitStream* received, IIncomingClient* sender) : mID(0),
        mType(packetType), mSender(sender)
        {

        }

        void IMessage::packEverything(Support::CBitStream& out) const
        {
            static Common::U32 sLastPacketID = 0;

            out << mType << (sLastPacketID++);
            // TODO (Robert MacGregor#9): Sequencing?
            // mID = sLastPacketID;
        }

        void IMessage::unpack(Support::CBitStream& in)
        {
            in >> mType;
            in >> mID;
        }

        Common::U32 IMessage::getType(void) const { return mType; }

        Common::U32 IMessage::getID(void) const { return mID; }

        size_t IMessage::getMinimumPacketPayloadLength(void) const
        {
            return sizeof(Common::U32) * 2;
        }

        size_t IMessage::getRequiredMemory(void) const
        {
            return sizeof(Common::U32) * 2;
        }
    } // End Namespace Net
} // End Namespace Kiaro

