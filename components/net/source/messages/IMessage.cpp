/**
 *  @file IMessage.cpp
 *  @brief Include file defining the Packet class.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#include <net/messages/IMessage.hpp>

namespace Kiaro
{
    namespace Net
    {
        namespace Messages
        {
            IMessage::IMessage(const Common::U32& packetType, Support::CBitStream* received , CIncomingClient* sender) : mID(0),
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
                in >> mID;
                in >> mType;
            }

            const Common::U32& IMessage::getType(void) const { return mType; }

            const Common::U32& IMessage::getID(void) const { return mID; }

            Common::U32 IMessage::getMinimumPacketPayloadLength(void)
            {
                return sizeof(Common::U32) * 2;
            }

            size_t IMessage::getRequiredMemory(void)
            {
                return sizeof(Common::U32) * 2;
            }
        } // End NameSpace Messages
    } // End Namespace Net
} // End Namespace Kiaro

