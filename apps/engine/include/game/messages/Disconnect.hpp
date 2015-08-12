/**
 *  @file Disconnect.hpp
 *  @brief The common include file for the Kiaro application defining things such as error values.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_GAME_MESSAGES_DISCONNECT_HPP_
#define _INCLUDE_KIARO_GAME_MESSAGES_DISCONNECT_HPP_

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
            class Disconnect : public Net::IMessage
            {
                // Public Methods
                public:
                    Disconnect(Support::CBitStream *in = NULL, Net::CClient *sender = NULL) : Net::IMessage(MESSAGE_DISCONNECT, in, sender)
                    {

                    }

                    void writeTo(Support::CBitStream &out) const
                    {
                        out << mReason;

                        Net::IMessage::writeTo(out);
                    }

                    void extractFrom(Support::CBitStream &in)
                    {
                        if (in.getWrittenLength() <= getMinimumPacketPayloadLength())
                            throw std::underflow_error("Unable to unpack Disconnect packet; too small of a payload!");

                        in >> mReason;
                    }

                    Common::U32 getMinimumPacketPayloadLength(void)
                    {
                        return sizeof(Common::U32);
                    }

                    size_t getRequiredMemory(void)
                    {
                        return mReason.length();
                    }

                // Public Members
                public:
                    Support::String mReason;
            };
        } // End NameSpace Packets
    } // End NameSpace Game
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_GAME_MESSAGES_DISCONNECT_HPP_
