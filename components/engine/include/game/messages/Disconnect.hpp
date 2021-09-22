/**
 *  @file Disconnect.hpp
 *  @brief The common include file for the Kiaro application defining things such as error values.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_KIARO_GAME_MESSAGES_DISCONNECT_HPP_
#define _INCLUDE_KIARO_GAME_MESSAGES_DISCONNECT_HPP_

#include <stdexcept>

#include <net/IMessage.hpp>

namespace Kiaro
{
    namespace Net
    {
        class IIncomingClient;
    }

    namespace Engine
    {
        namespace Game
        {
            namespace Messages
            {
                class Disconnect : public Net::IMessage
                {
                    // Public Methods
                    public:
                        Disconnect(Support::CBitStream* in = nullptr, Net::IIncomingClient* sender = nullptr) : IMessage(in, sender)
                        {
                        }

                        virtual void packEverything(Support::CBitStream& out) const
                        {
                            IMessage::packBaseData<Disconnect>(out);
                            out.writeString(mReason);
                        }

                        void unpack(Support::CBitStream& in)
                        {
                            if (in.getSize() - in.getPointer() < getMinimumPacketPayloadLength())
                            {
                                throw std::underflow_error("Unable to unpack Disconnect packet; too small of a payload!");
                            }

                            mReason = in.popString();
                        }

                        virtual size_t getMinimumPacketPayloadLength(void) const
                        {
                            return sizeof(Common::U32);
                        }

                        virtual size_t getRequiredMemory(void) const
                        {
                            return mReason.length();
                        }

                    // Public Members
                    public:
                        Support::String mReason;
                };
            } // End NameSpace Packets
        } // End NameSpace Game
    }
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_GAME_MESSAGES_DISCONNECT_HPP_
