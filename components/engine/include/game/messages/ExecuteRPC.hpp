/**
 *  @file ExecuteRPC.hpp
 *  @brief The common include file for the Kiaro application defining things such as error values.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#ifndef _INCLUDE_KIARO_GAME_MESSAGES_EXECUTERPC_HPP_
#define _INCLUDE_KIARO_GAME_MESSAGES_EXECUTERPC_HPP_

#include <stdexcept>

#include <game/messages/types.hpp>
#include <net/IMessage.hpp>

namespace Kiaro
{
    namespace Net
    {
        class IIncomingClient;
    }

    namespace Game
    {
        namespace Messages
        {
            class ExecuteRPC : public Net::IMessage
            {
                // Public Methods
                public:
                    ExecuteRPC(Support::CBitStream* in = NULL, Net::IIncomingClient* sender = NULL) : IMessage(TYPE_EXECUTERPC, in, sender)
                    {

                    }

                    virtual void packEverything(Support::CBitStream& out) const
                    {
                        IMessage::packEverything(out);
                        out << mName;
                    }

                    void unpack(Support::CBitStream &in)
                    {
                        if (in.getPointer() <= getMinimumPacketPayloadLength())
                            throw std::underflow_error("Unable to unpack ExecuteRPC packet; too small of a payload!");

                        in >> mName;
                    }

                    virtual size_t getMinimumPacketPayloadLength(void) const
                    {
                        return sizeof(Common::U32);
                    }

                    virtual size_t getRequiredMemory(void) const
                    {
                        return mName.length();
                    }

                // Public Members
                public:
                    Support::String mName;
            };
        } // End NameSpace Packets
    } // End NameSpace Game
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_GAME_MESSAGES_DISCONNECT_HPP_

