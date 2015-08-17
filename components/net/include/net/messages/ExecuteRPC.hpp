/**
 *  @file ExecuteRPC.hpp
 *  @brief The common include file for the Kiaro application defining things such as error values.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_GAME_MESSAGES_EXECUTERPC_HPP_
#define _INCLUDE_KIARO_GAME_MESSAGES_EXECUTERPC_HPP_

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
            class ExecuteRPC : public IMessage
            {
                // Public Methods
                public:
                    ExecuteRPC(Support::CBitStream* in = NULL, CIncomingClient* sender = NULL) : IMessage(TYPE_EXECUTERPC, in, sender)
                    {

                    }

                    virtual void packEverything(Support::CBitStream& out) const
                    {
                        out << mName;

                        IMessage::packEverything(out);
                    }

                    void unpack(Support::CBitStream &in)
                    {
                        if (in.getWrittenLength() <= getMinimumPacketPayloadLength())
                            throw std::underflow_error("Unable to unpack ExecuteRPC packet; too small of a payload!");

                        in >> mName;
                    }

                    Common::U32 getMinimumPacketPayloadLength(void)
                    {
                        return sizeof(Common::U32);
                    }

                    size_t getRequiredMemory(void)
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

