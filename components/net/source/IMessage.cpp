/**
 *  @file IMessage.cpp
 *  @brief Include file defining the Packet class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <net/IMessage.hpp>

namespace Kiaro
{
    namespace Net
    {
        IMessage::IMessage(Support::CBitStream* received, IIncomingClient* sender) : mID(0), mType(0), mSender(sender)
        {
        }

        void IMessage::packEverything(Support::CBitStream& out) const
        {

        }

        void IMessage::unpack(Support::CBitStream& in)
        {
            in >> mType;
            in >> mID;
        }

        Common::U32 IMessage::getType(void) const
        {
            return mType;
        }

        Common::U32 IMessage::getID(void) const
        {
            return mID;
        }

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

