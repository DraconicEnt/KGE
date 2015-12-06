/**
 *  @file Disconnect.hpp
 *  @brief The common include file for the Kiaro application defining things such as error values.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2015 Draconic Entity
 */

#ifndef _INCLUDE_KIARO_GAME_MESSAGES_DISCONNECT_HPP_
#define _INCLUDE_KIARO_GAME_MESSAGES_DISCONNECT_HPP_

#include <stdexcept>

#include <net/messages/types.hpp>
#include <net/messages/IMessage.hpp>

namespace Kiaro
{
    namespace Net
    {
        class CIncomingClient;

        namespace Messages
        {
            class Disconnect : public IMessage
            {
                // Public Methods
                public:
                    Disconnect(Support::CBitStream *in = NULL, CIncomingClient *sender = NULL) : IMessage(TYPE_DISCONNECT, in, sender)
                    {

                    }

                    virtual void packEverything(Support::CBitStream& out) const
                    {
                        out << mReason;

                        IMessage::packEverything(out);
                    }

                    void unpack(Support::CBitStream &in)
                    {
                        if (in.getPointer() <= getMinimumPacketPayloadLength())
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
